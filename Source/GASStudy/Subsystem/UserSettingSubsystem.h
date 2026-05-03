// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Save/Sound/Data/SoundSaveData.h"
#include "UserSettingSubsystem.generated.h"

/**
* 
*/
UCLASS()
class GASSTUDY_API UUserSettingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UUserSettingSubsystem();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	void InitSoundSetting(UWorld* World, const UWorld::InitializationValues IVS);

public:
	// Benchmark
	UFUNCTION(BlueprintCallable)
	void ApplyHardwareBenchmarkResults();
	UFUNCTION(BlueprintCallable)
	void RunHardwareBenchmark(int32 WorkScale, float CPUMultiplier, float GPUMultiplier);
	UFUNCTION(BlueprintCallable)
	void SetBenchmarkFallbackValues();

	// Resolution
	UFUNCTION(BlueprintCallable)
	void ApplyNonResolutionSettings();
	UFUNCTION(BlueprintCallable)
	void ApplyResolutionSettings(bool bCheckForCommandLineOverrides);
	UFUNCTION(BlueprintCallable)
	FIntPoint GetDefaultResolution();
	UFUNCTION(BlueprintCallable)
	FIntPoint GetDesktopResolution();
	UFUNCTION(BlueprintCallable)
	FIntPoint GetLastConfirmedScreenResolution();
	UFUNCTION(BlueprintCallable)
	FIntPoint GetScreenResolution();
	UFUNCTION(BlueprintCallable)
	bool GetIsScreenResolutionDirty();
	UFUNCTION(BlueprintCallable)
	void SetScreenResolution(FIntPoint Resolution);


	bool GetSupportedFullscreenResolutions(TArray<FIntPoint>& Resolutions);
	FText ConvertResolutionToText(const FIntPoint& Resolution);

	// Resolution Scale
	UFUNCTION(BlueprintCallable)
	float GetDefaultResolutionScale();
	UFUNCTION(BlueprintCallable)
	float GetRecommendedResolutionScale();
	UFUNCTION(BlueprintCallable)
	float GetResolutionScaleNormalized();
	UFUNCTION(BlueprintCallable)
	void GetResolutionScaleInfo(float& CurrentScaleNormalized, float& CurrentScaleValue, float& MinScaleValue, float& MaxScaleValue);
	UFUNCTION(BlueprintCallable)
	void SetResolutionScaleNormalized(float NewScaleNormalized);
	UFUNCTION(BlueprintCallable)
	void SetResolutionScaleValue(float NewScaleValue);

	// HDR 
	UFUNCTION(BlueprintCallable)
	void EnableHDR_DisplayOutput(bool Enable, int32 DisplayNits);
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentHDR_DisplayNits();
	UFUNCTION(BlueprintCallable)
	int32 GetIsHDR_Enabled();
	UFUNCTION(BlueprintCallable)
	bool GetSupportHDR_DisplayOutput();

	// Antial Aliasing
	UFUNCTION(BlueprintCallable)
	int32 GetAntiAliasingQuality();
	UFUNCTION(BlueprintCallable)
	void SetAntiAliasingQuality(int32 Value);

	// Audio Quality
	UFUNCTION(BlueprintCallable)
	int32 GetAudioQualityLevel();
	UFUNCTION(BlueprintCallable)
	void SetAudioQualityLevel(int32 Value);


	// Floliage
	UFUNCTION(BlueprintCallable)
	int32 GetFoliageQuality();
	UFUNCTION(BlueprintCallable)
	void SetFoliageQuality(int32 Value);

	// Frame Rate
	UFUNCTION(BlueprintCallable)
	int32 GetFramePace();
	UFUNCTION(BlueprintCallable)
	float GetFrameRateLimit();
	UFUNCTION(BlueprintCallable)
	void SetFrameRateLimit(float Value);

	// Fullscreen Mode 
	UFUNCTION(BlueprintCallable)
	EWindowMode::Type GetDefaultWindowMode();
	UFUNCTION(BlueprintCallable)
	EWindowMode::Type GetFullscreenMode();
	UFUNCTION(BlueprintCallable)
	EWindowMode::Type GetLastConfirmedFullscreenMode();
	UFUNCTION(BlueprintCallable)
	EWindowMode::Type GetPreferredFullscreenMode();
	UFUNCTION(BlueprintCallable)
	bool GetIsFullscreenModeDirty();
	UFUNCTION(BlueprintCallable)
	void SetFullscreenMode(EWindowMode::Type InFullScreenMode);

	// Global Illumination
	UFUNCTION(BlueprintCallable)
	int32 GetGlobalIlluminationQuality();
	UFUNCTION(BlueprintCallable)
	void SetGlobalIlluminationQuality(int32 Value);

	// Overall
	UFUNCTION(BlueprintCallable)
	int32 GetOverallScalabilityLevel();
	UFUNCTION(BlueprintCallable)
	void SetOverallScalabilityLevel(int32 Value);

	// Post Process
	UFUNCTION(BlueprintCallable)
	int32 GetPostProcessQuality();
	UFUNCTION(BlueprintCallable)
	void SetPostProcessQuality(int32 Value);

	// Reflection
	UFUNCTION(BlueprintCallable)
	int32 GetReflectionQuality();
	UFUNCTION(BlueprintCallable)
	void SetReflectionQuality(int32 Value);

	// Shading 
	UFUNCTION(BlueprintCallable)
	int32 GetShadingQuality();
	UFUNCTION(BlueprintCallable)
	void SetShadingQuality(int32 Value);

	// Shadow
	UFUNCTION(BlueprintCallable)
	int32 GetShadowQuality();
	UFUNCTION(BlueprintCallable)
	void SetShadowQuality(int32 Value);

	// Texture
	UFUNCTION(BlueprintCallable)
	int32 GetTextureQuality();
	UFUNCTION(BlueprintCallable)
	void SetTextureQuality(int32 Value);

	// View Distance
	UFUNCTION(BlueprintCallable)
	int32 GetViewDistanceQuality();
	UFUNCTION(BlueprintCallable)
	void SetViewDistanceQuality(int32 Value);

	// Visual Effect
	UFUNCTION(BlueprintCallable)
	int32 GetVisualEffectQuality();
	UFUNCTION(BlueprintCallable)
	void SetVisualEffectQuality(int32 Value);

	// Dynamic Resoulution
	UFUNCTION(BlueprintCallable)
	bool GetIsDynamicResolutionDirty();
	UFUNCTION(BlueprintCallable)
	bool GetIsDynamicResolutionEnabled();
	UFUNCTION(BlueprintCallable)
	void SetDynamicResolutionEnabled(bool Enable);

	// V-Sync
	UFUNCTION(BlueprintCallable)
	bool GetIsVsyncDirty();
	UFUNCTION(BlueprintCallable)
	bool GetIsVsyncEnabled();
	UFUNCTION(BlueprintCallable)
	void SetVsyncEnabled(bool Enable);

	// 일반 
	UFUNCTION(BlueprintCallable)
	void ApplySettings(bool CheckForCommandLineOverrides);
	UFUNCTION(BlueprintCallable)
	void ConfirmVideoMode();
	UFUNCTION(BlueprintCallable)
	FIntPoint GetDefaultWindowPosition();
	UFUNCTION(BlueprintCallable)
	int32 GetSyncInterval();
	UFUNCTION(BlueprintCallable)
	bool GetIsDirty();
	UFUNCTION(BlueprintCallable)
	void LoadSettings(bool ForceReload);
	UFUNCTION(BlueprintCallable)
	void ResetToCurrentSettings();
	UFUNCTION(BlueprintCallable)
	void RevertVideoMode(bool RevertManually);
	UFUNCTION(BlueprintCallable)
	void SaveSettings();
	UFUNCTION(BlueprintCallable)
	void SetToDefaults();
	UFUNCTION(BlueprintCallable)
	void ValidateSettings();
	UFUNCTION(BlueprintCallable)
	void CacheLastConfirmVideoMode();

	// 사운드 
	void SetSoundsDefault(FSoundSaveData& SoundSaveData);
	
	void SetSounds(const FSoundSaveData& SoundSaveData);

	void SaveSound();

	const FSoundSaveData GetSavedSoundData();


	void Apply_AllVolume();
	//void Apply_MasterVolume();
	void Apply_AmbientVolume();
	void Apply_SFXVolume();
	void Apply_UIVolume();

private:
	UPROPERTY()
	TObjectPtr<UGameUserSettings> mGUS;

	EWindowMode::Type mLastConfirmedFullscreenMode;
	FIntPoint mLastConfirmedScreenResolution;

	// 사운드 
	FSoundSaveData mSoundSaveData;

private:
	UPROPERTY()
	TObjectPtr<class USoundMix> mSoundMixClass;

	//UPROPERTY()
	//TObjectPtr<USoundClass> mMasterSoundClass;

	UPROPERTY()
	TObjectPtr<USoundClass> mBGMSoundClass;

	UPROPERTY()
	TObjectPtr<USoundClass> mSFXSoundClass;

	UPROPERTY()
	TObjectPtr<USoundClass> mUISoundClass;


	UPROPERTY()
	TWeakObjectPtr<UWorld> mWorld;

	FTimerHandle InitTimer;
};
