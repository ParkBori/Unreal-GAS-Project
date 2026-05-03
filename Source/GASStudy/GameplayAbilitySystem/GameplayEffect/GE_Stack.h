// ───────────────────────────────
// File: GE_Stack.h
// Desc: 시간, 스택형 디버프 / 데미지 / 버프 
// ───────────────────────────────
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_Stack.generated.h"

/**
 * 시간 타이머 Stack
 */
UCLASS()
class GASSTUDY_API UGE_Stack : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_Stack();
};

/**
 * 디버프
 */
UCLASS()
class GASSTUDY_API UGE_DebuffStack : public UGE_Stack
{
	GENERATED_BODY()

public:
	UGE_DebuffStack();
};

/**
* 시간 조정
*/
UCLASS()
class GASSTUDY_API UGE_ScalableStack : public UGE_Stack
{
	GENERATED_BODY()

public:
	UGE_ScalableStack();
};

/**
* 시간 조정이 가능한 디버프
*/
UCLASS(Abstract)
class GASSTUDY_API UGE_ScalableDebuffStack : public UGE_ScalableStack
{
	GENERATED_BODY()

public:
	UGE_ScalableDebuffStack();
};

/**
* 출혈 상태 (스택 크기 곱 데미지)
*/
UCLASS()
class GASSTUDY_API UGE_BleedingState : public UGE_DebuffStack
{
	GENERATED_BODY()

public:
	UGE_BleedingState();
};

/**
* 화상 상태 (고정 데미지)
*/
UCLASS()
class GASSTUDY_API UGE_BurnState : public UGE_ScalableDebuffStack
{
	GENERATED_BODY()

public:
	UGE_BurnState();
};

/**
* 독 상태 (최대 체력 비례)
*/
UCLASS()
class GASSTUDY_API UGE_PoisonState : public UGE_ScalableDebuffStack
{
	GENERATED_BODY()

public:
	UGE_PoisonState();
};
