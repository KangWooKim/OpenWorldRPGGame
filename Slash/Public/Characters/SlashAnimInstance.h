// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "SlashAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	// �ִϸ��̼� �ʱ�ȭ �Լ�
	virtual void NativeInitializeAnimation() override;

	// �ִϸ��̼� ������Ʈ �Լ�
	// @param DeltaTime: ���� �����Ӱ� ���� ������ ������ �ð� ����
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	// ������ ĳ���Ϳ� ���� ����
	UPROPERTY(BlueprintReadOnly)
		class ASlashCharacter* SlashCharacter;

	// ������ ĳ������ �̵� ������Ʈ�� ���� ����
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		class UCharacterMovementComponent* SlashCharacterMovement;

	// ĳ������ ���� �ӵ�
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float GroundSpeed;

	// ĳ���Ͱ� ���߿� �ִ��� ���θ� ��Ÿ���� �÷���
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		bool IsFalling;

	// ĳ������ ���� ���� (��: �ȱ�, �޸���, ���� ��)
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		ECharacterState CharacterState;

	// ĳ������ ���� ���� ���� (��: ����, ȸ��, ���� ��)
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		EActionState ActionState;

	// ĳ������ ���� ���� ����
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		TEnumAsByte<EDeathPose> DeathPose;

};
