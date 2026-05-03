
#pragma once

#include "CoreMinimal.h"

/**
* 작성한 로그를 원하는 쪽에, 로그가 남겨진 파일/줄 / 추가내용 
* ex) MY_LOG(Warning, "CurrentIdx : %d -> TargetIdx : %d", CurrentIdx, TargetIdx);
*/
#define MY_LOG(Verbosity, Format, ...) \
UE_LOG(LogTemp, Verbosity, TEXT("[%s : %d] : %s"), TEXT(__FUNCTION__), __LINE__, *FString::Printf(TEXT(Format), ##__VA_ARGS__))

/**
* 컴파일시 경고로 뜨게 만듦. 꼭 처리해야하는 사항에 대해 사용
* ex) TODO("어빌리티 시스템 / 블루프린라이브러리 태그에셋인터페이스 캐스팅이 너무 많이 일어난다. 추후, 타겟의 어빌리티시스템을 캐싱하여 캐스팅 횟수를 줄일 수 있도록 수정이 필요하다");
*/
#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define TODO(msg) __pragma(message(__FILE__ "(" STRING(__LINE__) "): [TODO] " msg))

//CCT 글로벌 어빌리티 세팅에서, 글로벌 어트리뷰트를 설정해서, 일관된 어빌리티 시스템 프레임워크를 설계중임
//
//1. 커브데이터를 만들기
//2. 컴포짓 커브데이터 넣기
//3. 글로벌어빌리티세팅에서 디폴트어트리뷰트셋으로 지정
//4. 이 값을 기반으로 플레이어나, 캐릭터 스폰시 초기화 하기 (이때 지정된 레벨을 고려해서 선택해야함)


//
//template<typename T>
//FString ArrayToString(const TArray<T>& Array)
//{
//    FString Result;
//    for (int32 i = 0; i < Array.Num(); ++i)
//    {
//        Result += LexToString(Array[i]);
//
//        if (i < Array.Num() - 1)
//        {
//            Result += TEXT(", ");
//        }
//    }
//    return Result;
//}
