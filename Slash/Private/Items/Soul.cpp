// Fill out your copyright notice in the Description page of Project Settings.

#include "interfaces/PickupInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/Soul.h"

// �� �����Ӹ��� ȣ��Ǵ� �Լ�
void ASoul::Tick(float DeltaTime)  // DeltaTime: ���� �����Ӱ��� �ð� ��
{
    Super::Tick(DeltaTime);

    // ���� ������ Z ��ġ�� ������
    const double LocationZ = GetActorLocation().Z;

    // ���� ���� ��ġ�� ���ϴ� ��ġ���� ���ٸ�
    if (LocationZ > DesiredZ) {
        // Z �������� ������ ��ġ�� ���
        const FVector DeltaLocation = FVector(0.f, 0.f, DriftRate * DeltaTime);

        // ���� ��ġ��ŭ ���͸� ������
        AddActorWorldOffset(DeltaLocation);
    }
}

// ���� �����̳� ������ �� ȣ��Ǵ� �Լ�
void ASoul::BeginPlay()
{
    Super::BeginPlay();

    // �浹 Ž���� ���� �������� ������ ����
    const FVector Start = GetActorLocation();
    const FVector End = Start - FVector(0.f, 0.f, 2000.f);

    // ��ü Ÿ�� ����
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

    // ������ ���� ����
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetOwner());

    // �浹 ����� ������ ����
    FHitResult HitResult;

    // ���� Ʈ���̽� ����
    UKismetSystemLibrary::LineTraceSingleForObjects(
        this,
        Start,
        End,
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::None,
        HitResult,
        true
    );

    // ���ϴ� Z ��ġ�� �浹 ��ġ�� ����
    DesiredZ = HitResult.ImpactPoint.Z + 50.f;
}

// ������ �̺�Ʈ �߻� �� ȣ��Ǵ� �Լ�
void ASoul::OnSphereOverlap(
    UPrimitiveComponent* OverlappedComponent,  // �������� ������Ʈ
    AActor* OtherActor,  // �������� �ٸ� ����
    UPrimitiveComponent* OtherComp,  // �������� �ٸ� ������Ʈ
    int32 OtherBodyIndex,  // �������� �ٸ� �ٵ� �ε���
    bool bFromSweep,  // �������κ����� ������ ����
    const FHitResult& SweepResult)  // ���� ���
{
    // �ٸ� ���͸� �Ⱦ� �������̽��� ĳ����
    IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

    // ���� ĳ������ �����ߴٸ�
    if (PickupInterface) {
        // �ҿ��� �߰�
        PickupInterface->AddSouls(this);

        // �Ⱦ� ȿ���� ���� ����
        SpawnPickupSystem();
        SpawnPickupSound();

        // �� ���͸� �ı�
        Destroy();
    }
}

