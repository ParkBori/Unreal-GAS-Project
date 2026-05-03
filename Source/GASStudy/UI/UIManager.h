// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIInfo.h"
#include "BaseUserWidget.h"

class GASSTUDY_API CUIManager
{
private:
	CUIManager();
	~CUIManager();

private:
	TMap<FString, TObjectPtr<UBaseUserWidget>>	mWidgetMap;

public:
	void AddWidget(const FString& Name, UBaseUserWidget* Widget);
	void DeleteWidget(const FString& Name);
	template <typename T>
	T* FindWidget(const FString& Name)
	{
		return Cast<T>(mWidgetMap.FindRef(Name));
	}

private:
	static CUIManager* mInst;

public:
	static CUIManager* GetInst()
	{
		if (!mInst)
			mInst = new CUIManager;
		return mInst;
	}

	static void DestroyInst()
	{
		if (mInst)
		{
			delete mInst;
			mInst = nullptr;
		}
	}
};
