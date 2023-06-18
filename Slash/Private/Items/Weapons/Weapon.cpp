// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"

// AWeapon의 생성자 함수
AWeapon::AWeapon()
{
    // 무기 박스 컴포넌트를 초기화하고 루트 컴포넌트에 붙입니다.
    WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
    WeaponBox->SetupAttachment(GetRootComponent());

    // 무기 박스의 충돌을 비활성화하고, 모든 채널에 대한 응답을 Overlap으로 설정합니다.
    // 단, ECC_Pawn 채널에 대해서는 응답을 Ignore로 설정합니다.
    WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECCollisionResponse::ECR_Ignore);

    // 박스 트레이스 시작점을 설정하고 루트 컴포넌트에 붙입니다.
    BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
    BoxTraceStart->SetupAttachment(GetRootComponent());

    // 박스 트레이스 끝점을 설정하고 루트 컴포넌트에 붙입니다.
    BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
    BoxTraceEnd->SetupAttachment(GetRootComponent());
}

// 게임이 시작하거나 스폰될 때 호출되는 함수
void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    // 무기 박스가 겹친 경우 호출되는 함수를 등록합니다.
    WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

// 무기를 장착하는 함수
void AWeapon::Equip(
    USceneComponent* InParent,  // 장착할 부모 컴포넌트
    FName InSocketName,  // 소켓 이름
    AActor* NewOwner,  // 새로운 오너 액터
    APawn* NewInstigator  // 새로운 설치자
)
{
    // 아이템 상태를 장착 상태로 변경합니다.
    ItemState = EItemState::EIS_Equipped;

    // 오너와 설치자를 설정합니다.
    SetOwner(NewOwner);
    SetInstigator(NewInstigator);

    // 소켓에 메시를 붙입니다.
    AttachMeshToSocket(InParent, InSocketName);

    // 구형 충돌을 비활성화합니다.
    DisableSphereCollision();

    // 장착 사운드를 재생합니다.
    PlayEquipSound();

    // 불씨 효과를 비활성화합니다.
    DeactivateEmbers();
}

// 불씨 효과를 비활성화하는 함수
void AWeapon::DeactivateEmbers()
{
    // 아이템 효과가 있을 경우 비활성화합니다.
    if (ItemEffect)
    {
        ItemEffect->Deactivate();
    }
}

// 구형 충돌을 비활성화하는 함수
void AWeapon::DisableSphereCollision()
{
    // Sphere가 있을 경우 충돌을 비활성화합니다.
    if (Sphere)
    {
        Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

// 장착 사운드를 재생하는 함수
void AWeapon::PlayEquipSound()
{
    // 장착 사운드가 있을 경우 재생합니다.
    if (EquipSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            EquipSound,
            GetActorLocation()
        );
    }
}


// 메쉬를 특정 소켓에 연결하는 함수입니다.
void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
    // 부착 규칙을 정의합니다.
    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);

    // 메쉬를 지정한 부모 컴포넌트의 지정한 소켓에 연결합니다.
    ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

// 오버랩 이벤트가 발생했을 때 호출되는 함수입니다.
void AWeapon::OnBoxOverlap(
    UPrimitiveComponent* OverlappedComponent,  // 오버랩이 발생한 컴포넌트
    AActor* OtherActor,  // 오버랩이 발생한 다른 액터
    UPrimitiveComponent* OtherComp,  // 오버랩이 발생한 다른 컴포넌트
    int32 OtherBodyIndex,  // 오버랩이 발생한 다른 바디의 인덱스
    bool bFromSweep,  // 스윕으로부터의 오버랩 발생 여부
    const FHitResult& SweepResult  // 스윕 결과
)
{
    // 같은 타입의 액터인 경우 함수를 반환합니다.
    if (ActorIsSameType(OtherActor)) return;

    // 박스 히트를 정의합니다.
    FHitResult BoxHit;

    // 박스 트레이스를 수행합니다.
    BoxTrace(BoxHit);

    // 히트한 액터가 있는 경우
    if (BoxHit.GetActor())
    {
        // 히트한 액터가 같은 타입인 경우 함수를 반환합니다.
        if (ActorIsSameType(BoxHit.GetActor())) return;

        // 히트한 액터에 대해 데미지를 적용합니다.
        UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());

        // 히트 이벤트를 실행합니다.
        ExecuteGetHit(BoxHit);

        // 필드를 생성합니다.
        CreateFields(BoxHit.ImpactPoint);
    }
}

// 주어진 액터가 같은 타입인지 확인하는 함수입니다.
bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
    // 오너 액터와 주어진 액터가 모두 "Enemy" 태그를 가지고 있는지 확인합니다.
    return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

// 히트 이벤트를 실행하는 함수입니다.
void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
    // 히트 인터페이스를 캐스팅합니다.
    IHitinterface* HitInterface = Cast<IHitinterface>(BoxHit.GetActor());

    // 인터페이스가 있는 경우 히트 이벤트를 실행합니다.
    if (HitInterface)
    {
        HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
    }
}

// 박스 트레이스를 수행하는 함수입니다.
void AWeapon::BoxTrace(FHitResult& BoxHit)
{
    // 시작점과 끝점을 정의합니다.
    const FVector Start = BoxTraceStart->GetComponentLocation();
    const FVector End = BoxTraceEnd->GetComponentLocation();

    // 무시할 액터들의 배열을 정의합니다.
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    ActorsToIgnore.Add(GetOwner());

    // 무시할 액터들을 배열에 추가합니다.
    for (AActor* Actor : IgnoreActors)
    {
        ActorsToIgnore.AddUnique(Actor);
    }

    // 박스 트레이스를 수행합니다.
    UKismetSystemLibrary::BoxTraceSingle(
        this,
        Start,
        End,
        BoxTraceExtent,
        BoxTraceStart->GetComponentRotation(),
        ETraceTypeQuery::TraceTypeQuery1,
        false,
        ActorsToIgnore,
        bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
        BoxHit,
        true
    );

    // 무시할 액터들에 히트한 액터를 추가합니다.
    IgnoreActors.AddUnique(BoxHit.GetActor());
}
