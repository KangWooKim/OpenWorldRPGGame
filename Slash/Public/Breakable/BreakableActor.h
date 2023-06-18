// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitinterface
{
	// 생성된 바디
	GENERATED_BODY()

public:
	// 이 액터의 속성들에 대한 기본값 설정
	ABreakableActor();

	// 충격에 대한 처리를 하는 가상 함수. FVector 타입의 충격 지점과 AActor 타입의 히터를 인자로 받습니다.
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	// 게임이 시작하거나 생성될 때 호출되는 가상 함수
	virtual void BeginPlay() override;

	// GeometryCollection 컴포넌트. 언제든지 볼 수 있으며, 블루프린트에서 읽고 쓸 수 있습니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UGeometryCollectionComponent* GeometryCollection;

	// Capsule 컴포넌트. 언제든지 볼 수 있으며, 블루프린트에서 읽고 쓸 수 있습니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCapsuleComponent* Capsule;

public:
	// 매 프레임마다 호출되는 가상 함수. 프레임 시간인 DeltaTime을 인자로 받습니다.
	virtual void Tick(float DeltaTime) override;

private:
	// Breakable 속성을 가진 보물 클래스 배열. 에디터에서 어디서나 수정할 수 있습니다.
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
		TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	// 이 액터가 부서졌는지 나타내는 불린 값. 초기값은 false입니다.
	bool bBroken = false;
};
