// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"

CUIManager* CUIManager::mInst = nullptr;

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
}

void CUIManager::AddWidget(const FString& Name, UBaseUserWidget* Widget)
{
	if (mWidgetMap.Contains(Name))
		return;

	mWidgetMap.Add(Name, Widget);
}

void CUIManager::DeleteWidget(const FString& Name)
{
	if (!mWidgetMap.Contains(Name))
		return;

	mWidgetMap.Remove(Name);
}
