// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystem/UserSettingSubsystem.h"

#include "GameFramework/GameUserSettings.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Subsystem/SaveSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "Sound/SoundMix.h"

#include "Sound/SoundClass.h"

UUserSettingSubsystem::UUserSettingSubsystem()
{
	ConstructorHelpers::FObjectFinder<USoundMix> SoundMixAsset(TEXT("/Game/GASStudy/Sounds/SM_Master.SM_Master"));
	if (SoundMixAsset.Succeeded())
	{
		mSoundMixClass = SoundMixAsset.Object;
	}

	//ConstructorHelpers::FObjectFinder<USoundClass> MasterSoundClassAsset(TEXT("/Game/GASStudy/Sounds/SC_Master.SC_Master"));
	//if (MasterSoundClassAsset.Succeeded())
	//{
	//	mMasterSoundClass = MasterSoundClassAsset.Object;
	//}

	ConstructorHelpers::FObjectFinder<USoundClass> BGMSoundClassAsset(TEXT("/Game/GASStudy/Sounds/SC_BGM.SC_BGM"));
	if (BGMSoundClassAsset.Succeeded())
	{
		mBGMSoundClass = BGMSoundClassAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundClass> SFXSoundClassAsset(TEXT("/Game/GASStudy/Sounds/SC_SFX.SC_SFX"));
	if (SFXSoundClassAsset.Succeeded())
	{
		mSFXSoundClass = SFXSoundClassAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundClass> UISoundClassAsset(TEXT("/Game/GASStudy/Sounds/SC_UI.SC_UI"));
	if (UISoundClassAsset.Succeeded())
	{
		mUISoundClass = UISoundClassAsset.Object;
	}
}

void UUserSettingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (GEngine)
	{
		mGUS = GEngine->GetGameUserSettings();
		checkf(mGUS, TEXT("게임 유저 세팅 초기화 실패"));
	}

	if (!IsValid(mGUS))
	{
		return;
	}

	mGUS->SetScreenResolution(mGUS->GetScreenResolution());

	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UUserSettingSubsystem::InitSoundSetting);
}

void UUserSettingSubsystem::InitSoundSetting(UWorld* World, const UWorld::InitializationValues IVS)
{
	if (!IsValid(mSoundMixClass))
	{
		return;
	}

	if (!World || !World->IsGameWorld())
	{
		return;
	}

	auto* SS = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (!IsValid(SS))
	{
		return;
	}

	UGameplayStatics::ClearSoundMixModifiers(World);
	UGameplayStatics::PushSoundMixModifier(World, mSoundMixClass);
	
	mWorld = World;

	mSoundSaveData = SS->GetSavedSoundData();
	Apply_AllVolume();

}

void UUserSettingSubsystem::ApplyHardwareBenchmarkResults()
{
	mGUS->ApplyHardwareBenchmarkResults();
}

void UUserSettingSubsystem::RunHardwareBenchmark(int32 WorkScale, float CPUMultiplier, float GPUMultiplier)
{
	mGUS->RunHardwareBenchmark(WorkScale, CPUMultiplier, GPUMultiplier);
}

void UUserSettingSubsystem::SetBenchmarkFallbackValues()
{
	mGUS->SetBenchmarkFallbackValues();
}

// Resolution
void UUserSettingSubsystem::ApplyNonResolutionSettings()
{
	mGUS->ApplyNonResolutionSettings();
}

void UUserSettingSubsystem::ApplyResolutionSettings(bool bCheckForCommandLineOverrides)
{
	mGUS->ApplyResolutionSettings(bCheckForCommandLineOverrides);
}

FIntPoint UUserSettingSubsystem::GetDefaultResolution()
{
	return mGUS->GetDefaultResolution();
}

FIntPoint UUserSettingSubsystem::GetDesktopResolution()
{
	return mGUS->GetDesktopResolution();
}

FIntPoint UUserSettingSubsystem::GetLastConfirmedScreenResolution()
{
	return mGUS->GetLastConfirmedScreenResolution();
}

FIntPoint UUserSettingSubsystem::GetScreenResolution()
{
	return mGUS->GetScreenResolution();
}

bool UUserSettingSubsystem::GetIsScreenResolutionDirty()
{
	return mGUS->IsScreenResolutionDirty();
}

void UUserSettingSubsystem::SetScreenResolution(FIntPoint Resolution)
{
	mGUS->SetScreenResolution(Resolution);
}

bool UUserSettingSubsystem::GetSupportedFullscreenResolutions(TArray<FIntPoint>& Resolutions)
{
	return UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
}

FText UUserSettingSubsystem::ConvertResolutionToText(const FIntPoint& Resolution)
{
#define LOCTEXT_NAMESPACE "ResolutionText"

	// 기존의 포맷팅을 사용하면 숫자 사이에 , 가 들어가게 된다. 
	// return FText::Format(LOCTEXT("ResolutionText", "{0}x{1}"), Resolution.X, Resolution.Y);
	FNumberFormattingOptions NoGrouping;
	NoGrouping.SetUseGrouping(false);
	return FText::Format(
		LOCTEXT("ResolutionText", "{0}x{1}"),
		FText::AsNumber(Resolution.X, &NoGrouping),
		FText::AsNumber(Resolution.Y, &NoGrouping)
	);

#undef LOCTEXT_NAMESPACE 
}

// Resolution Scale
float UUserSettingSubsystem::GetDefaultResolutionScale()
{
	return mGUS->GetDefaultResolutionScale();
}

float UUserSettingSubsystem::GetRecommendedResolutionScale()
{
	return mGUS->GetRecommendedResolutionScale();
}

float UUserSettingSubsystem::GetResolutionScaleNormalized()
{
	return mGUS->GetResolutionScaleNormalized();
}

void UUserSettingSubsystem::GetResolutionScaleInfo(float& CurrentScaleNormalized, float& CurrentScaleValue, float& MinScaleValue, float& MaxScaleValue)
{
	mGUS->GetResolutionScaleInformationEx(CurrentScaleNormalized, CurrentScaleValue, MinScaleValue, MaxScaleValue);
}

void UUserSettingSubsystem::SetResolutionScaleNormalized(float NewScaleNormalized)
{
	mGUS->SetResolutionScaleNormalized(NewScaleNormalized);
}
void UUserSettingSubsystem::SetResolutionScaleValue(float NewScaleValue)
{
	mGUS->SetResolutionScaleValueEx(NewScaleValue);
}

// HDR 
void UUserSettingSubsystem::EnableHDR_DisplayOutput(bool Enable, int32 DisplayNits)
{
	return mGUS->EnableHDRDisplayOutput(Enable, DisplayNits);
}

int32 UUserSettingSubsystem::GetCurrentHDR_DisplayNits()
{
	return mGUS->GetCurrentHDRDisplayNits();
}

int32 UUserSettingSubsystem::GetIsHDR_Enabled()
{
	return mGUS->GetCurrentHDRDisplayNits();
}

bool UUserSettingSubsystem::GetSupportHDR_DisplayOutput()
{
	return mGUS->SupportsHDRDisplayOutput();
}

// Antial Aliasing
int32 UUserSettingSubsystem::GetAntiAliasingQuality()
{
	return mGUS->GetAntiAliasingQuality();
}

void UUserSettingSubsystem::SetAntiAliasingQuality(int32 Value)
{
	mGUS->SetAntiAliasingQuality(Value);
}

// Audio Quality
int32 UUserSettingSubsystem::GetAudioQualityLevel()
{
	return mGUS->GetAudioQualityLevel();
}

void UUserSettingSubsystem::SetAudioQualityLevel(int32 Value)
{
	mGUS->SetAudioQualityLevel(Value);
}

// Floliage
int32 UUserSettingSubsystem::GetFoliageQuality()
{
	return mGUS->GetFoliageQuality();
}

void UUserSettingSubsystem::SetFoliageQuality(int32 Value)
{
	mGUS->SetFoliageQuality(Value);
}

// Frame Rate
int32 UUserSettingSubsystem::GetFramePace()
{
	return mGUS->GetFramePace();
}

float UUserSettingSubsystem::GetFrameRateLimit()
{
	return mGUS->GetFrameRateLimit();
}

void UUserSettingSubsystem::SetFrameRateLimit(float Value)
{
	mGUS->SetFrameRateLimit(Value);
}

// Fullscreen Mode 
EWindowMode::Type UUserSettingSubsystem::GetDefaultWindowMode()
{
	return mGUS->GetDefaultWindowMode();
}

EWindowMode::Type UUserSettingSubsystem::GetFullscreenMode()
{
	return mGUS->GetFullscreenMode();
}

EWindowMode::Type UUserSettingSubsystem::GetLastConfirmedFullscreenMode()
{
	return mGUS->GetLastConfirmedFullscreenMode();
}

EWindowMode::Type UUserSettingSubsystem::GetPreferredFullscreenMode()
{
	return mGUS->GetPreferredFullscreenMode();
}

bool UUserSettingSubsystem::GetIsFullscreenModeDirty()
{
	return mGUS->IsFullscreenModeDirty();
}

void UUserSettingSubsystem::SetFullscreenMode(EWindowMode::Type InFullScreenMode)
{
	mGUS->SetFullscreenMode(InFullScreenMode);
}

int32 UUserSettingSubsystem::GetGlobalIlluminationQuality()
{
	return mGUS->GetGlobalIlluminationQuality();
}

void UUserSettingSubsystem::SetGlobalIlluminationQuality(int32 Value)
{
	mGUS->SetGlobalIlluminationQuality(Value);
}

// Overall
int32 UUserSettingSubsystem::GetOverallScalabilityLevel()
{
	return mGUS->GetOverallScalabilityLevel();
}

void UUserSettingSubsystem::SetOverallScalabilityLevel(int32 Value)
{
	mGUS->SetOverallScalabilityLevel(Value);
}

// Post Process
int32 UUserSettingSubsystem::GetPostProcessQuality()
{
	return mGUS->GetPostProcessingQuality();
}

void UUserSettingSubsystem::SetPostProcessQuality(int32 Value)
{
	mGUS->SetPostProcessingQuality(Value);
}

// Reflection
int32 UUserSettingSubsystem::GetReflectionQuality()
{
	return mGUS->GetReflectionQuality();
}

void UUserSettingSubsystem::SetReflectionQuality(int32 Value)
{
	mGUS->SetReflectionQuality(Value);
}

// Shading 
int32 UUserSettingSubsystem::GetShadingQuality()
{
	return mGUS->GetShadingQuality();
}

void UUserSettingSubsystem::SetShadingQuality(int32 Value)
{
	mGUS->SetShadingQuality(Value);
}

// Shadow
int32 UUserSettingSubsystem::GetShadowQuality()
{
	return mGUS->GetShadowQuality();
}

void UUserSettingSubsystem::SetShadowQuality(int32 Value)
{
	mGUS->SetShadowQuality(Value);
}

// Texture
int32 UUserSettingSubsystem::GetTextureQuality()
{
	return mGUS->GetTextureQuality();
}

void UUserSettingSubsystem::SetTextureQuality(int32 Value)
{
	mGUS->SetTextureQuality(Value);
}

// View Distance
int32 UUserSettingSubsystem::GetViewDistanceQuality()
{
	return mGUS->GetViewDistanceQuality();
}

void UUserSettingSubsystem::SetViewDistanceQuality(int32 Value)
{
	mGUS->SetViewDistanceQuality(Value);
}

int32 UUserSettingSubsystem::GetVisualEffectQuality()
{
	return mGUS->GetVisualEffectQuality();
}

void UUserSettingSubsystem::SetVisualEffectQuality(int32 Value)
{
	mGUS->SetVisualEffectQuality(Value);
}

bool UUserSettingSubsystem::GetIsDynamicResolutionDirty()
{
	return mGUS->IsDynamicResolutionDirty();
}

bool UUserSettingSubsystem::GetIsDynamicResolutionEnabled()
{
	return mGUS->IsDynamicResolutionEnabled();
}

void UUserSettingSubsystem::SetDynamicResolutionEnabled(bool Enable)
{
	mGUS->SetDynamicResolutionEnabled(Enable);
}

bool UUserSettingSubsystem::GetIsVsyncDirty()
{
	return mGUS->IsVSyncDirty();
}

bool UUserSettingSubsystem::GetIsVsyncEnabled()
{
	return mGUS->IsVSyncEnabled();
}
void UUserSettingSubsystem::SetVsyncEnabled(bool Enable)
{
	mGUS->SetVSyncEnabled(Enable);
}

void UUserSettingSubsystem::ApplySettings(bool CheckForCommandLineOverrides) 
{
	mGUS->ApplySettings(CheckForCommandLineOverrides);
}

void UUserSettingSubsystem::ConfirmVideoMode()
{
	mGUS->ConfirmVideoMode();
}

FIntPoint UUserSettingSubsystem::GetDefaultWindowPosition()
{
	return mGUS->GetDefaultWindowPosition();
}

int32 UUserSettingSubsystem::GetSyncInterval()
{
	return mGUS->GetFramePace();
}

bool UUserSettingSubsystem::GetIsDirty()
{
	return mGUS->IsDirty();
}

void UUserSettingSubsystem::LoadSettings(bool ForceReload)
{
	mGUS->LoadSettings(ForceReload);
}

void UUserSettingSubsystem::ResetToCurrentSettings()
{
	mGUS->ResetToCurrentSettings();
}

void UUserSettingSubsystem::RevertVideoMode(bool RevertManually)
{
	if (RevertManually)
	{
		SetFullscreenMode(mLastConfirmedFullscreenMode);
		SetScreenResolution(mLastConfirmedScreenResolution);
		ConfirmVideoMode();
	}
	else
	{
		mGUS->RevertVideoMode();
	}
}

void UUserSettingSubsystem::SaveSettings()
{
	mGUS->SaveSettings();
}

void UUserSettingSubsystem::SetToDefaults()
{
	mGUS->SetToDefaults();
}

void UUserSettingSubsystem::ValidateSettings()
{
	mGUS->ValidateSettings();
}

void UUserSettingSubsystem::CacheLastConfirmVideoMode()
{
	mLastConfirmedFullscreenMode = GetLastConfirmedFullscreenMode();
	mLastConfirmedScreenResolution = GetLastConfirmedScreenResolution();
}

void UUserSettingSubsystem::SetSoundsDefault(FSoundSaveData& SoundSaveData)
{
	mSoundSaveData = SoundSaveData = DEFAULT_SOUND_VALUE;
	Apply_AllVolume();
}

void UUserSettingSubsystem::SetSounds(const FSoundSaveData& SoundSaveData)
{
	mSoundSaveData = SoundSaveData;
	Apply_AllVolume();
}

void UUserSettingSubsystem::SaveSound()
{
	auto* SS = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (!SS)
	{
		return;
	}

	SS->SaveSound(mSoundSaveData);
}

const FSoundSaveData UUserSettingSubsystem::GetSavedSoundData()
{
	// TODO: 여기에 return 문을 삽입합니다.
	auto* SS = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (!SS)
	{
		return FSoundSaveData();
	}

	return SS->GetSavedSoundData();
}

void UUserSettingSubsystem::Apply_AllVolume()
{
	//Apply_MasterVolume();
	Apply_AmbientVolume();
	Apply_SFXVolume();
	Apply_UIVolume();
	if (mWorld.IsValid())
	{
		UGameplayStatics::PushSoundMixModifier(mWorld.Get(), mSoundMixClass);
	}
}

//void UUserSettingSubsystem::Apply_MasterVolume()
//{
//	if (mWorld.IsValid())
//	{
//		UGameplayStatics::SetSoundMixClassOverride(mWorld.Get(), mSoundMixClass, mMasterSoundClass, mSoundSaveData.Master / 100.f, 1.0f, 0.f, true);
//	}
//}

void UUserSettingSubsystem::Apply_AmbientVolume()
{
	if (mWorld.IsValid())
	{
	
		UGameplayStatics::SetSoundMixClassOverride(mWorld.Get(), mSoundMixClass, mBGMSoundClass, mSoundSaveData.BGM / 100.f, 1.0f, 0.f, true);
	}
}

void UUserSettingSubsystem::Apply_SFXVolume()
{
	if (mWorld.IsValid())
	{
		UGameplayStatics::SetSoundMixClassOverride(mWorld.Get(), mSoundMixClass, mSFXSoundClass, mSoundSaveData.SFX / 100.f, 1.0f, 0.f, true);
	}
}

void UUserSettingSubsystem::Apply_UIVolume()
{
	if (mWorld.IsValid())
	{
		UGameplayStatics::SetSoundMixClassOverride(mWorld.Get(), mSoundMixClass, mUISoundClass, mSoundSaveData.UI / 100.f, 1.0f, 0.f, true);
	}
}