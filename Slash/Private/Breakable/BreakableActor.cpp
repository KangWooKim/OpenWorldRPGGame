// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"

// �⺻ �� ����
ABreakableActor::ABreakableActor()
{
	// �� ���͸� �� �����Ӹ��� Tick() �Լ��� ȣ���ϵ��� �����մϴ�. �ʿ����� �ʴٸ� �̸� ���� ������ ������ �� �ֽ��ϴ�.
	PrimaryActorTick.bCanEverTick = false;

	// GeometryCollection ������Ʈ�� �⺻ ���������Ʈ�� �����ϰ� �ʱ�ȭ�մϴ�.
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));

	// GeometryCollection ������Ʈ�� �� ������ ��Ʈ ������Ʈ�� �����մϴ�.
	SetRootComponent(GeometryCollection);

	// GeometryCollection�� ��ħ �̺�Ʈ ������ Ȱ��ȭ�մϴ�.
	GeometryCollection->SetGenerateOverlapEvents(true);

	// ī�޶�� �÷��̾� ĳ����(Pawn)�� ���� �浹 ������ �����ϵ��� �����մϴ�.
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECCollisionResponse::ECR_Ignore);

	// Capsule ������Ʈ�� �⺻ ���������Ʈ�� �����ϰ� �ʱ�ȭ�մϴ�.
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));

	// Capsule ������Ʈ�� ��Ʈ ������Ʈ�� ÷���մϴ�.
	Capsule->SetupAttachment(GetRootComponent());

	// ��� ä�ο� ���� �浹 ������ �����ϵ��� �����ϰ�, �÷��̾� ĳ����(Pawn)�� ���� �浹�� �����ϵ��� �����մϴ�.
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECCollisionResponse::ECR_Block);
}

// ��� ������ ���͸� ���ڷ� �޾�, ��ݿ� ���� ó���� ����ϴ� �޼����Դϴ�.
void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	// �̹� �μ��� ���, �߰� ó���� ���� �ʽ��ϴ�.
	if (bBroken) return;
	// �μ��� ���·� �����մϴ�.
	bBroken = true;
	UWorld* World = GetWorld();
	// ���� ����� ���� Ŭ������ ���� ���, ������ ��ġ�� ������ �����մϴ�.
	if (World && TreasureClasses.Num() > 0) {
		FVector Location = GetActorLocation();
		Location.Z += 75.f;

		// �����ϰ� ������ ���� Ŭ������ ������� ������ �����մϴ�.
		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);

		World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}
}

// ������ �����ϰų� ������ �� ȣ��Ǵ� �޼����Դϴ�.
void ABreakableActor::BeginPlay()
{
	// �θ� Ŭ������ BeginPlay() �Լ��� ȣ���մϴ�.
	Super::BeginPlay();

}

// �� �����Ӹ��� ȣ��Ǵ� �޼����Դϴ�.
void ABreakableActor::Tick(float DeltaTime)
{
	// �θ� Ŭ������ Tick() �Լ��� ȣ���մϴ�.
	Super::Tick(DeltaTime);

}


