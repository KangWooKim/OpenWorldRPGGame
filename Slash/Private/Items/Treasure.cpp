// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "interfaces/PickupInterface.h"


// ���Ǿ� ������Ʈ�� �ٸ� ������Ʈ�� ������ �� ȣ��Ǵ� �Լ�
void ATreasure::OnSphereOverlap(
    UPrimitiveComponent* OverlappedComponent,  // ��ģ �� ��ü�� ������Ʈ
    AActor* OtherActor,  // ��ģ �ٸ� ����
    UPrimitiveComponent* OtherComp,  // ��ģ �ٸ� ������Ʈ
    int32 OtherBodyIndex,  // ��ģ �ٸ� �ٵ��� �ε���
    bool bFromSweep,  // �������κ����� ���������� ����
    const FHitResult& SweepResult  // ������ ���
)
{
    // �ٸ� ���Ͱ� IPickupInterface�� �����ϰ� �ִ��� Ȯ���մϴ�.
    IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

    // �����Ǿ� �ִٸ�
    if (PickupInterface) {
        // �ش� �������̽��� AddGold �޼ҵ带 ȣ���Ͽ� ��ȭ�� �߰��մϴ�. �� ��ü�� �����մϴ�.
        PickupInterface->AddGold(this);

        // �Ⱦ� ���带 ����մϴ�.
        SpawnPickupSound();

        // �� ��ü�� �ı��մϴ�.
        Destroy();
    }
}
