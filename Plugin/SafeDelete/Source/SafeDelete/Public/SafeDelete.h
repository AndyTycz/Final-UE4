// Copyright © 2020 Isaac Montagne All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "IAssetRegistry.h"

class FAssetRegistryModule;

class SAFEDELETE_API FContentBrowserMenuExtension : public TSharedFromThis<FContentBrowserMenuExtension>
{

    IAssetRegistry& AssetRegistry;
    TArray<FString> Paths;

public:
    explicit FContentBrowserMenuExtension(const TArray<FString>& SelectedPaths);

    void AddMenuEntry(FMenuBuilder& MenuBuilder);
    void OnButtonClicked();

    bool RecursiveDeleteSelection(const FName& Asset, TArray<FName>& ToDelete, TArray<FName>& CheckedAssets);
    bool IsInsidePaths(const FName& Asset);

	void GetDirectoriesRecursively(const FString& InPath, TArray<FString>& Directories);
	bool DeleteEmptyFolderFromDisk(const FString& InPathToDelete);

};

class SAFEDELETE_API FSafeDeleteModule : public IModuleInterface
{

    TSharedPtr<FContentBrowserMenuExtension> Extension;
	TSharedPtr<class FSlateStyleSet> StyleSet;

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

    TSharedRef<FExtender> CustomExtender(const TArray<FString>& Path);

};