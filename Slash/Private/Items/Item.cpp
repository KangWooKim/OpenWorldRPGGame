// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "interfaces/PickupInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

// �⺻ �� ����
AItem::AItem()
{
	// �� ���Ͱ� �� �����Ӹ��� Tick()�� ȣ���ϵ��� �����մϴ�. �ʿ� ���ٸ� �̸� ���� ������ ���˴ϴ�.
	PrimaryActorTick.bCanEverTick = true;

	// ������ �޽� ������Ʈ ���� �� ����
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	// ���� ������Ʈ ���� �� �θ� ������Ʈ�� ����
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	// ����Ʈ ���� �� �θ� ������Ʈ�� ����
	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(GetRootComponent());
}

// ������ �����ϰų� ������ �� ȣ���
void AItem::BeginPlay()
{
	Super::BeginPlay();

	// ���� ������Ʈ�� ������ ���۰� ���ῡ ���� �̺�Ʈ ���ε�
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

}

// ������ ���� ���� ���
float AItem::TransformedSin()
{
	// ������ ���� ���� ���Ͽ� ������ ���� ���� ��ȯ
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

// ������ �ڻ��� ���� ���
float AItem::TransformedCos()
{
	// ������ �ڻ��� ���� ���Ͽ� ������ �ڻ��� ���� ��ȯ
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

// ���� ������Ʈ�� ������ ���� �� ȣ���
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �������� ���ͷκ��� �Ⱦ� �������̽��� ����
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface) {
		// �������� �������� ���� ���������� ����
		PickupInterface->SetOverlappingItem(this);
	}
}

// ���� ������Ʈ�� ������ ���� �� ȣ���
void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// ������ ������ ���ͷκ��� �Ⱦ� �������̽��� ����
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface) {
		// �������� �������� nullptr�� ����
		PickupInterface->SetOverlappingItem(nullptr);
	}
}

// �Ⱦ� ����Ʈ�� ����
void AItem::SpawnPickupSystem()
{
	if (PickupEffect) {
		// �������� ��ġ���� �Ⱦ� ����Ʈ�� ����
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation()
		);
	}
}


// �Ⱦ� �Ҹ��� �����ϴ� �޼ҵ�
void AItem::SpawnPickupSound()
{
	// �Ⱦ� �Ҹ��� �����Ǿ� ���� ���
	if (PickupSound) {
		// �������� ��ġ���� �Ⱦ� �Ҹ��� ����
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			Pickup Sound,  // ������ �Ҹ��� ������Ʈ
			GetActorLocation()  // �Ҹ��� ������ ��ġ
		);
	}
}

// �� �����Ӹ��� ȣ��Ǵ� �޼ҵ�
void AItem::Tick(float DeltaTime)  // DeltaTime: ���� �����Ӱ� ���� ������ ������ �ð� ����
{
	Super::Tick(DeltaTime);

	// ���� �ð��� ��Ÿ Ÿ�� �߰�
	RunningTime += DeltaTime;
	// �������� ���°� ȣ���� ������ ���
	if (ItemState == EItemState::EIS_Hovering) {
		// �������� ���� �����¿� ���� ��ȯ ���� �߰��Ͽ� �������� ���Ʒ��� �����̰� ��
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}
