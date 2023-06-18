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
	// �� ���� �Ӽ��� ���� �⺻ �� ����
	ABird();

protected:
	// ���� ���� �� �Ǵ� ������ �� ȣ��
	virtual void BeginPlay() override;
	// ���� �̵��� ���� �Լ�
	void MoveForward(float Value);  // ������ ���Դϴ�.
	// ȸ���� ���� �Լ�
	void Turn(float Value);  // ȸ���� ���Դϴ�.
	// ���Ʒ��� ���� �ϴ� �Լ�
	void LookUp(float Value);  // ���Ʒ��� �� ���Դϴ�.

public:
	// �� �����Ӹ��� ȣ��
	virtual void Tick(float DeltaTime) override;  // ���� ƽ ������ �ð��Դϴ�.

	// �Է¿� ����� ���ε��ϱ� ���� ȣ��
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;  // �÷��̾� �Է� ������Ʈ�Դϴ�.

private:
	// ĸ�� ������Ʈ. ������ ������ �浹�� ó��
	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* Capsule;

	// ���� �޽�. �ð��� ǥ���� ���
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* BirdMesh;

	// ī�޶� ��. ī�޶�� �÷��̾� ���� ������ �Ÿ��� ����
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;

	// �� ī�޶�. �÷��̾��� ������ ǥ��
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

};
