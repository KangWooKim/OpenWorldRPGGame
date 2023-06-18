// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// �� Ŭ������ �⺻ �������Դϴ�.
ABird::ABird()
{
    // �� ���ʹ� ƽ �̺�Ʈ�� ���� �� �ֽ��ϴ�.
    PrimaryActorTick.bCanEverTick = true;

    // ĸ�� ������Ʈ�� �����ϰ� �����մϴ�.
    Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    Capsule->SetCapsuleHalfHeight(20.f);
    Capsule->SetCapsuleRadius(15.f);
    SetRootComponent(Capsule);

    // �� �޽��� �����ϰ� ��Ʈ ������Ʈ�� �����մϴ�.
    BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
    BirdMesh->SetupAttachment(GetRootComponent());

    // ī�޶� ���� �����ϰ� �����մϴ�.
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 300.f;

    // �� ī�޶� �����ϰ� ī�޶� �տ� �����մϴ�.
    ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    ViewCamera->SetupAttachment(CameraBoom);

    // �÷��̾� 0�� �ڵ����� �����ϵ��� �����մϴ�.
    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// �÷��� ���� �� ȣ��Ǵ� �޼ҵ��Դϴ�.
void ABird::BeginPlay()
{
    Super::BeginPlay();
}

// ���� �����ϵ��� �ϴ� �޼ҵ��Դϴ�.
void ABird::MoveForward(float Value)  // ������ ���Դϴ�.
{
    if (Controller && (Value != 0.f))
    {
        // ������ ���� ������ ����ϴ�.
        FVector Forward = GetActorForwardVector();
        // ���� �������� �����Դϴ�.
        AddMovementInput(Forward, Value);
    }
}

// ���� ȸ����Ű�� �޼ҵ��Դϴ�.
void ABird::Turn(float Value)  // ȸ���� ���Դϴ�.
{
    AddControllerYawInput(Value);
}

// ���� ���Ʒ��� ���� �ϴ� �޼ҵ��Դϴ�.
void ABird::LookUp(float Value)  // ���Ʒ��� �� ���Դϴ�.
{
    AddControllerPitchInput(Value);
}

// �� ƽ���� ȣ��Ǵ� �޼ҵ��Դϴ�.
void ABird::Tick(float DeltaTime)  // ���� ƽ ������ �ð��Դϴ�.
{
    Super::Tick(DeltaTime);
}

// �Է°� ����� �����ϴ� �޼ҵ��Դϴ�.
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)  // �÷��̾� �Է� ������Ʈ�Դϴ�.
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // "MoveForward", "Turn", "LookUp" �Է¿� ������ �Լ��� �����մϴ�.
    PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABird::MoveForward);
    PlayerInputComponent->BindAxis(FName("Turn"), this, &ABird::Turn);
    PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABird::LookUp);
}
