// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "interfaces/PickupInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

// 기본 값 설정
AItem::AItem()
{
	// 이 액터가 매 프레임마다 Tick()을 호출하도록 설정합니다. 필요 없다면 이를 끄면 성능이 향상됩니다.
	PrimaryActorTick.bCanEverTick = true;

	// 아이템 메시 컴포넌트 생성 및 설정
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	// 구형 컴포넌트 생성 및 부모 컴포넌트에 부착
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	// 이펙트 생성 및 부모 컴포넌트에 부착
	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(GetRootComponent());
}

// 게임이 시작하거나 스폰될 때 호출됨
void AItem::BeginPlay()
{
	Super::BeginPlay();

	// 구형 컴포넌트의 오버랩 시작과 종료에 대한 이벤트 바인딩
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

}

// 변형된 사인 값을 계산
float AItem::TransformedSin()
{
	// 진폭에 사인 값을 곱하여 변형된 사인 값을 반환
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

// 변형된 코사인 값을 계산
float AItem::TransformedCos()
{
	// 진폭에 코사인 값을 곱하여 변형된 코사인 값을 반환
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

// 구형 컴포넌트의 오버랩 시작 시 호출됨
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 오버랩된 액터로부터 픽업 인터페이스를 얻음
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface) {
		// 오버랩된 아이템을 현재 아이템으로 설정
		PickupInterface->SetOverlappingItem(this);
	}
}

// 구형 컴포넌트의 오버랩 종료 시 호출됨
void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 오버랩 종료한 액터로부터 픽업 인터페이스를 얻음
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface) {
		// 오버랩된 아이템을 nullptr로 설정
		PickupInterface->SetOverlappingItem(nullptr);
	}
}

// 픽업 이펙트를 스폰
void AItem::SpawnPickupSystem()
{
	if (PickupEffect) {
		// 아이템의 위치에서 픽업 이펙트를 스폰
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation()
		);
	}
}


// 픽업 소리를 생성하는 메소드
void AItem::SpawnPickupSound()
{
	// 픽업 소리가 설정되어 있을 경우
	if (PickupSound) {
		// 아이템의 위치에서 픽업 소리를 생성
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			Pickup Sound,  // 생성할 소리의 오브젝트
			GetActorLocation()  // 소리를 생성할 위치
		);
	}
}

// 매 프레임마다 호출되는 메소드
void AItem::Tick(float DeltaTime)  // DeltaTime: 이전 프레임과 현재 프레임 사이의 시간 차이
{
	Super::Tick(DeltaTime);

	// 실행 시간에 딸타 타임 추가
	RunningTime += DeltaTime;
	// 아이템의 상태가 호버링 상태일 경우
	if (ItemState == EItemState::EIS_Hovering) {
		// 아이템의 월드 오프셋에 사인 변환 값을 추가하여 아이템이 위아래로 움직이게 함
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}
