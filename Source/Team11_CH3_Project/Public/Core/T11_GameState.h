#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "T11_GameState.generated.h"

UCLASS()
class TEAM11_CH3_PROJECT_API AT11_GameState : public AGameState
{
	GENERATED_BODY()
	
	/*
	스테이지 시작
	-> 플레이어 스폰 지점에 스폰
	-> 웨이브 시작 -> 몬스터 스폰 지점에 스폰
	-> 웨이브의 시간이 끝 or 웨이브의 몬스터 전부 처치 or 몬스터 일부 처치 -> 다음 웨이브 시작
	-> 모든 웨이브 클리어 -> 포탈 생성
	
	몬스터 처치시 점수 증가
	*/



	// 스테이지 시작
	void StartLevel();
	// 웨이브 시작
	void StartNextWave();

};
