// Copyright © 2020 Isaac Montagne All Rights Reserved.

#include "SafeDelete.h"


#include "AssetRegistryModule.h"
#include "ContentBrowserModule.h"
#include "IAssetRegistry.h"
#include "Misc/MessageDialog.h"
#include "ObjectTools.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "FSafeDeleteModule"

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )


void FSafeDeleteModule::StartupModule()
{
	// TODO: Clean this and move it somewhere else
	const auto ResourcesDir = IPluginManager::Get().FindPlugin(TEXT("SafeDelete"))->GetBaseDir() / "Resources";
	const FVector2D Icon16x16(16.f, 16.f);

	StyleSet = MakeShareable(new FSlateStyleSet("SafeDelete"));
	StyleSet->SetContentRoot(ResourcesDir);
	StyleSet->Set("ContentBrowser.SafeDelete", new FSlateImageBrush(ResourcesDir / "Icon16.png", Icon16x16));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);


    auto& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
    auto& MenuExtenderDelegates = ContentBrowserModule.GetAllPathViewContextMenuExtenders();

    MenuExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FSafeDeleteModule::CustomExtender));
}

void FSafeDeleteModule::ShutdownModule()
{
}

TSharedRef<FExtender> FSafeDeleteModule::CustomExtender(const TArray<FString>& Path)
{
    Extension = MakeShareable(new FContentBrowserMenuExtension(Path));

    TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
    MenuExtender->AddMenuExtension(
        "Delete", EExtensionHook::After,
        TSharedPtr<FUICommandList>(),
        FMenuExtensionDelegate::CreateSP(Extension.ToSharedRef(), &FContentBrowserMenuExtension::AddMenuEntry)
    );

    return MenuExtender.ToSharedRef();
}

FContentBrowserMenuExtension::FContentBrowserMenuExtension(const TArray<FString>& SelectedPaths)
    : AssetRegistry(FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get())
{
    Paths = SelectedPaths;
}

void FContentBrowserMenuExtension::AddMenuEntry(FMenuBuilder& MenuBuilder)
{
    MenuBuilder.AddMenuEntry(
        FText::FromString("Safe Delete..."),
        FText::FromString("Delete all these folders' assets which are not referenced outside them."),
        FSlateIcon("SafeDelete", "ContentBrowser.SafeDelete"),
        FUIAction(FExecuteAction::CreateSP(this, &FContentBrowserMenuExtension::OnButtonClicked))
    );
}

void FContentBrowserMenuExtension::OnButtonClicked()
{
	const auto ModalResult =
		FMessageDialog::Open(EAppMsgType::YesNoCancel, FText::FromString("Do you want to delete empty folders?"));

	if (ModalResult == EAppReturnType::Cancel)
	{
		return;
	}

	const auto DeleteEmptyFolders = ModalResult == EAppReturnType::Yes;

    TArray<FAssetData> AssetDataList;

    // GetAssetsInPaths
    {
        // Form a filter from the paths
        FARFilter Filter;
        Filter.bRecursivePaths = true;
        for (int32 PathIdx = 0; PathIdx < Paths.Num(); ++PathIdx)
        {
            new (Filter.PackagePaths) FName(*Paths[PathIdx]);
        }

        // Query for a list of assets in the selected paths
        AssetRegistry.GetAssets(Filter, AssetDataList);
    }

    TArray<FName> ToDelete;
    for (auto Asset : AssetDataList)
    {
        TArray<FName> CheckedAssets;
        RecursiveDeleteSelection(Asset.PackageName, ToDelete, CheckedAssets);
    }

    TArray<FAssetData> AssetsToDelete;

    for (auto Name : ToDelete)
    {
        TArray<FAssetData> Assets;
        AssetRegistry.GetAssetsByPackageName(Name, Assets);

        for (auto Asset : Assets)
        {
            AssetsToDelete.Add(Asset);
        }
    }

    if (AssetsToDelete.Num() == 0)
    {
        FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("There is no asset to delete."));
    }

    ObjectTools::DeleteAssets(AssetsToDelete);

	if (DeleteEmptyFolders)
	{
		TArray<FString> Directories;

		for (auto Path : Paths)
		{
			GetDirectoriesRecursively(Path, Directories);
		}

		for (auto Directory : Directories)
		{
			UE_LOG(LogTemp, Error, TEXT("%s"), *Directory);

			if (DeleteEmptyFolderFromDisk(Directory))
			{
				AssetRegistry.RemovePath(Directory);
			}
		}
	}
}

bool FContentBrowserMenuExtension::RecursiveDeleteSelection(const FName& Asset, TArray<FName>& ToDelete, TArray<FName>& CheckedAssets)
{
    if (CheckedAssets.Contains(Asset))
    {
        return true;
    }

    CheckedAssets.Add(Asset);

    if (!IsInsidePaths(Asset))
    {
        return false;
    }

    TArray<FName> Referencers;
    AssetRegistry.GetReferencers(Asset, Referencers);

    for (auto Referencer : Referencers)
    {
        if (!RecursiveDeleteSelection(Referencer, ToDelete, CheckedAssets))
        {
            return false;
        }
    }

    ToDelete.Add(Asset);
    return true;
}

bool FContentBrowserMenuExtension::IsInsidePaths(const FName& Asset)
{
    for (auto Path : Paths)
    {
        if (FPaths::IsUnderDirectory(Asset.ToString(), Path))
        {
            return true;
        }
    }

    return false;
}

void FContentBrowserMenuExtension::GetDirectoriesRecursively(const FString& InPath, TArray<FString>& Directories)
{
	struct FFolderVisitor : IPlatformFile::FDirectoryVisitor
	{
		TArray<FString>* Directories;

		FFolderVisitor(TArray<FString>* Directories)
			: Directories(Directories) {}

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			if (bIsDirectory)
			{
				Directories->Add(FilenameOrDirectory);
			}

			return true;
		}
	};

	FString PathToDeleteOnDisk;
	if (FPackageName::TryConvertLongPackageNameToFilename(InPath, PathToDeleteOnDisk))
	{
		FFolderVisitor FolderVisitor(&Directories);
		IFileManager::Get().IterateDirectoryRecursively(*PathToDeleteOnDisk, FolderVisitor);
	}
}

bool FContentBrowserMenuExtension::DeleteEmptyFolderFromDisk(const FString& InPathToDelete)
{
	struct FEmptyFolderVisitor : IPlatformFile::FDirectoryVisitor
	{
		bool bIsEmpty;

		FEmptyFolderVisitor()
            : bIsEmpty(true)
		{
		}

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			if (!bIsDirectory)
			{
				bIsEmpty = false;
				return false; // abort searching
			}

			return true; // continue searching
		}
	};

	// Look for files on disk in case the folder contains things not tracked by the asset registry
	FEmptyFolderVisitor EmptyFolderVisitor;
	IFileManager::Get().IterateDirectoryRecursively(*InPathToDelete, EmptyFolderVisitor);

	if (EmptyFolderVisitor.bIsEmpty)
	{
		return IFileManager::Get().DeleteDirectory(*InPathToDelete, false, true);
	}

	return false;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSafeDeleteModule, SafeDelete)