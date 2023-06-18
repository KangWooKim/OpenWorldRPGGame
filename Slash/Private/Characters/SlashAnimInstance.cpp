// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


// 애니메이션 초기화 함수
void USlashAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // 소유자를 SlashCharacter 형식으로 캐스팅
    SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());
    if (SlashCharacter) {
        // 캐릭터의 이동 정보를 가져옵니다.
        SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
    }
}

// 애니메이션 업데이트 함수
// @param DeltaTime: 이전 프레임과 현재 프레임 사이의 시간 간격
void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (SlashCharacterMovement) {
        // 캐릭터의 이동 속도를 계산합니다. 
        GroundSpeed ​​ = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);

        // 캐릭터가 공중에 있는지 확인합니다.
        IsFalling = SlashCharacterMovement->IsFalling();

        // 캐릭터의 현재 상태를 가져옵니다.
        CharacterState = SlashCharacter->GetCharacterState();

        // 캐릭터의 행동 상태를 가져옵니다.
        ActionState = SlashCharacter->GetActionState();

        // 캐릭터의 죽음 포즈를 가져옵니다.
        DeathPose = SlashCharacter->GetDeathPose();
    }
}

