// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// 새 클래스의 기본 생성자입니다.
ABird::ABird()
{
    // 이 액터는 틱 이벤트를 받을 수 있습니다.
    PrimaryActorTick.bCanEverTick = true;

    // 캡슐 컴포넌트를 생성하고 설정합니다.
    Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    Capsule->SetCapsuleHalfHeight(20.f);
    Capsule->SetCapsuleRadius(15.f);
    SetRootComponent(Capsule);

    // 새 메쉬를 생성하고 루트 컴포넌트에 부착합니다.
    BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
    BirdMesh->SetupAttachment(GetRootComponent());

    // 카메라 붐을 생성하고 설정합니다.
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 300.f;

    // 뷰 카메라를 생성하고 카메라 붐에 부착합니다.
    ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    ViewCamera->SetupAttachment(CameraBoom);

    // 플레이어 0을 자동으로 소유하도록 설정합니다.
    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// 플레이 시작 시 호출되는 메소드입니다.
void ABird::BeginPlay()
{
    Super::BeginPlay();
}

// 새가 전진하도록 하는 메소드입니다.
void ABird::MoveForward(float Value)  // 전진할 값입니다.
{
    if (Controller && (Value != 0.f))
    {
        // 액터의 전진 방향을 얻습니다.
        FVector Forward = GetActorForwardVector();
        // 전진 방향으로 움직입니다.
        AddMovementInput(Forward, Value);
    }
}

// 새를 회전시키는 메소드입니다.
void ABird::Turn(float Value)  // 회전할 값입니다.
{
    AddControllerYawInput(Value);
}

// 새를 위아래로 보게 하는 메소드입니다.
void ABird::LookUp(float Value)  // 위아래로 볼 값입니다.
{
    AddControllerPitchInput(Value);
}

// 각 틱마다 호출되는 메소드입니다.
void ABird::Tick(float DeltaTime)  // 지난 틱 이후의 시간입니다.
{
    Super::Tick(DeltaTime);
}

// 입력과 기능을 연결하는 메소드입니다.
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)  // 플레이어 입력 컴포넌트입니다.
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // "MoveForward", "Turn", "LookUp" 입력에 각각의 함수를 연결합니다.
    PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABird::MoveForward);
    PlayerInputComponent->BindAxis(FName("Turn"), this, &ABird::Turn);
    PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABird::LookUp);
}
