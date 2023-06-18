// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"

// 기본 값 설정
ABreakableActor::ABreakableActor()
{
	// 이 액터를 매 프레임마다 Tick() 함수를 호출하도록 설정합니다. 필요하지 않다면 이를 끄고 성능을 개선할 수 있습니다.
	PrimaryActorTick.bCanEverTick = false;

	// GeometryCollection 컴포넌트를 기본 서브오브젝트로 생성하고 초기화합니다.
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));

	// GeometryCollection 컴포넌트를 이 액터의 루트 컴포넌트로 설정합니다.
	SetRootComponent(GeometryCollection);

	// GeometryCollection에 겹침 이벤트 생성을 활성화합니다.
	GeometryCollection->SetGenerateOverlapEvents(true);

	// 카메라와 플레이어 캐릭터(Pawn)에 대한 충돌 반응을 무시하도록 설정합니다.
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECCollisionResponse::ECR_Ignore);

	// Capsule 컴포넌트를 기본 서브오브젝트로 생성하고 초기화합니다.
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));

	// Capsule 컴포넌트를 루트 컴포넌트에 첨부합니다.
	Capsule->SetupAttachment(GetRootComponent());

	// 모든 채널에 대한 충돌 응답을 무시하도록 설정하고, 플레이어 캐릭터(Pawn)에 대한 충돌은 차단하도록 설정합니다.
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECCollisionResponse::ECR_Block);
}

// 충격 지점과 히터를 인자로 받아, 충격에 대한 처리를 담당하는 메서드입니다.
void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	// 이미 부서진 경우, 추가 처리를 하지 않습니다.
	if (bBroken) return;
	// 부서짐 상태로 변경합니다.
	bBroken = true;
	UWorld* World = GetWorld();
	// 현재 월드와 보물 클래스가 있을 경우, 랜덤한 위치에 보물을 생성합니다.
	if (World && TreasureClasses.Num() > 0) {
		FVector Location = GetActorLocation();
		Location.Z += 75.f;

		// 랜덤하게 선택한 보물 클래스를 기반으로 보물을 생성합니다.
		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);

		World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}
}

// 게임이 시작하거나 생성될 때 호출되는 메서드입니다.
void ABreakableActor::BeginPlay()
{
	// 부모 클래스의 BeginPlay() 함수를 호출합니다.
	Super::BeginPlay();

}

// 매 프레임마다 호출되는 메서드입니다.
void ABreakableActor::Tick(float DeltaTime)
{
	// 부모 클래스의 Tick() 함수를 호출합니다.
	Super::Tick(DeltaTime);

}


