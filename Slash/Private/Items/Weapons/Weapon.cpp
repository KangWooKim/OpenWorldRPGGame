// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"

// AWeapon�� ������ �Լ�
AWeapon::AWeapon()
{
    // ���� �ڽ� ������Ʈ�� �ʱ�ȭ�ϰ� ��Ʈ ������Ʈ�� ���Դϴ�.
    WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
    WeaponBox->SetupAttachment(GetRootComponent());

    // ���� �ڽ��� �浹�� ��Ȱ��ȭ�ϰ�, ��� ä�ο� ���� ������ Overlap���� �����մϴ�.
    // ��, ECC_Pawn ä�ο� ���ؼ��� ������ Ignore�� �����մϴ�.
    WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECCollisionResponse::ECR_Ignore);

    // �ڽ� Ʈ���̽� �������� �����ϰ� ��Ʈ ������Ʈ�� ���Դϴ�.
    BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
    BoxTraceStart->SetupAttachment(GetRootComponent());

    // �ڽ� Ʈ���̽� ������ �����ϰ� ��Ʈ ������Ʈ�� ���Դϴ�.
    BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
    BoxTraceEnd->SetupAttachment(GetRootComponent());
}

// ������ �����ϰų� ������ �� ȣ��Ǵ� �Լ�
void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    // ���� �ڽ��� ��ģ ��� ȣ��Ǵ� �Լ��� ����մϴ�.
    WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

// ���⸦ �����ϴ� �Լ�
void AWeapon::Equip(
    USceneComponent* InParent,  // ������ �θ� ������Ʈ
    FName InSocketName,  // ���� �̸�
    AActor* NewOwner,  // ���ο� ���� ����
    APawn* NewInstigator  // ���ο� ��ġ��
)
{
    // ������ ���¸� ���� ���·� �����մϴ�.
    ItemState = EItemState::EIS_Equipped;

    // ���ʿ� ��ġ�ڸ� �����մϴ�.
    SetOwner(NewOwner);
    SetInstigator(NewInstigator);

    // ���Ͽ� �޽ø� ���Դϴ�.
    AttachMeshToSocket(InParent, InSocketName);

    // ���� �浹�� ��Ȱ��ȭ�մϴ�.
    DisableSphereCollision();

    // ���� ���带 ����մϴ�.
    PlayEquipSound();

    // �Ҿ� ȿ���� ��Ȱ��ȭ�մϴ�.
    DeactivateEmbers();
}

// �Ҿ� ȿ���� ��Ȱ��ȭ�ϴ� �Լ�
void AWeapon::DeactivateEmbers()
{
    // ������ ȿ���� ���� ��� ��Ȱ��ȭ�մϴ�.
    if (ItemEffect)
    {
        ItemEffect->Deactivate();
    }
}

// ���� �浹�� ��Ȱ��ȭ�ϴ� �Լ�
void AWeapon::DisableSphereCollision()
{
    // Sphere�� ���� ��� �浹�� ��Ȱ��ȭ�մϴ�.
    if (Sphere)
    {
        Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

// ���� ���带 ����ϴ� �Լ�
void AWeapon::PlayEquipSound()
{
    // ���� ���尡 ���� ��� ����մϴ�.
    if (EquipSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            EquipSound,
            GetActorLocation()
        );
    }
}


// �޽��� Ư�� ���Ͽ� �����ϴ� �Լ��Դϴ�.
void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
    // ���� ��Ģ�� �����մϴ�.
    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);

    // �޽��� ������ �θ� ������Ʈ�� ������ ���Ͽ� �����մϴ�.
    ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

// ������ �̺�Ʈ�� �߻����� �� ȣ��Ǵ� �Լ��Դϴ�.
void AWeapon::OnBoxOverlap(
    UPrimitiveComponent* OverlappedComponent,  // �������� �߻��� ������Ʈ
    AActor* OtherActor,  // �������� �߻��� �ٸ� ����
    UPrimitiveComponent* OtherComp,  // �������� �߻��� �ٸ� ������Ʈ
    int32 OtherBodyIndex,  // �������� �߻��� �ٸ� �ٵ��� �ε���
    bool bFromSweep,  // �������κ����� ������ �߻� ����
    const FHitResult& SweepResult  // ���� ���
)
{
    // ���� Ÿ���� ������ ��� �Լ��� ��ȯ�մϴ�.
    if (ActorIsSameType(OtherActor)) return;

    // �ڽ� ��Ʈ�� �����մϴ�.
    FHitResult BoxHit;

    // �ڽ� Ʈ���̽��� �����մϴ�.
    BoxTrace(BoxHit);

    // ��Ʈ�� ���Ͱ� �ִ� ���
    if (BoxHit.GetActor())
    {
        // ��Ʈ�� ���Ͱ� ���� Ÿ���� ��� �Լ��� ��ȯ�մϴ�.
        if (ActorIsSameType(BoxHit.GetActor())) return;

        // ��Ʈ�� ���Ϳ� ���� �������� �����մϴ�.
        UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());

        // ��Ʈ �̺�Ʈ�� �����մϴ�.
        ExecuteGetHit(BoxHit);

        // �ʵ带 �����մϴ�.
        CreateFields(BoxHit.ImpactPoint);
    }
}

// �־��� ���Ͱ� ���� Ÿ������ Ȯ���ϴ� �Լ��Դϴ�.
bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
    // ���� ���Ϳ� �־��� ���Ͱ� ��� "Enemy" �±׸� ������ �ִ��� Ȯ���մϴ�.
    return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

// ��Ʈ �̺�Ʈ�� �����ϴ� �Լ��Դϴ�.
void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
    // ��Ʈ �������̽��� ĳ�����մϴ�.
    IHitinterface* HitInterface = Cast<IHitinterface>(BoxHit.GetActor());

    // �������̽��� �ִ� ��� ��Ʈ �̺�Ʈ�� �����մϴ�.
    if (HitInterface)
    {
        HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
    }
}

// �ڽ� Ʈ���̽��� �����ϴ� �Լ��Դϴ�.
void AWeapon::BoxTrace(FHitResult& BoxHit)
{
    // �������� ������ �����մϴ�.
    const FVector Start = BoxTraceStart->GetComponentLocation();
    const FVector End = BoxTraceEnd->GetComponentLocation();

    // ������ ���͵��� �迭�� �����մϴ�.
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    ActorsToIgnore.Add(GetOwner());

    // ������ ���͵��� �迭�� �߰��մϴ�.
    for (AActor* Actor : IgnoreActors)
    {
        ActorsToIgnore.AddUnique(Actor);
    }

    // �ڽ� Ʈ���̽��� �����մϴ�.
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

    // ������ ���͵鿡 ��Ʈ�� ���͸� �߰��մϴ�.
    IgnoreActors.AddUnique(BoxHit.GetActor());
}
