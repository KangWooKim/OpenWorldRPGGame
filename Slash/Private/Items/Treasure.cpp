// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "interfaces/PickupInterface.h"


// 스피어 컴포넌트가 다른 컴포넌트와 겹쳤을 때 호출되는 함수
void ATreasure::OnSphereOverlap(
    UPrimitiveComponent* OverlappedComponent,  // 겹친 이 객체의 컴포넌트
    AActor* OtherActor,  // 겹친 다른 액터
    UPrimitiveComponent* OtherComp,  // 겹친 다른 컴포넌트
    int32 OtherBodyIndex,  // 겹친 다른 바디의 인덱스
    bool bFromSweep,  // 스윕으로부터의 오버랩인지 여부
    const FHitResult& SweepResult  // 스윕의 결과
)
{
    // 다른 액터가 IPickupInterface를 구현하고 있는지 확인합니다.
    IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

    // 구현되어 있다면
    if (PickupInterface) {
        // 해당 인터페이스의 AddGold 메소드를 호출하여 금화를 추가합니다. 이 객체를 전달합니다.
        PickupInterface->AddGold(this);

        // 픽업 사운드를 재생합니다.
        SpawnPickupSound();

        // 이 객체를 파괴합니다.
        Destroy();
    }
}
