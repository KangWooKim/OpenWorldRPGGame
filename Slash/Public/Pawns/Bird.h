// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SLASH_API ABird : public APawn
{
	GENERATED_BODY()

public:
	// 이 폰의 속성에 대한 기본 값 설정
	ABird();

protected:
	// 게임 시작 시 또는 스폰될 때 호출
	virtual void BeginPlay() override;
	// 전진 이동을 위한 함수
	void MoveForward(float Value);  // 전진할 값입니다.
	// 회전을 위한 함수
	void Turn(float Value);  // 회전할 값입니다.
	// 위아래로 보게 하는 함수
	void LookUp(float Value);  // 위아래로 볼 값입니다.

public:
	// 매 프레임마다 호출
	virtual void Tick(float DeltaTime) override;  // 지난 틱 이후의 시간입니다.

	// 입력에 기능을 바인드하기 위해 호출
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;  // 플레이어 입력 컴포넌트입니다.

private:
	// 캡슐 컴포넌트. 액터의 볼륨과 충돌을 처리
	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* Capsule;

	// 새의 메쉬. 시각적 표현을 담당
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* BirdMesh;

	// 카메라 붐. 카메라와 플레이어 간의 물리적 거리를 유지
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;

	// 뷰 카메라. 플레이어의 시점을 표현
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

};
