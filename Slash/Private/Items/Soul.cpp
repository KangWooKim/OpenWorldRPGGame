// Fill out your copyright notice in the Description page of Project Settings.

#include "interfaces/PickupInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/Soul.h"

// 매 프레임마다 호출되는 함수
void ASoul::Tick(float DeltaTime)  // DeltaTime: 이전 프레임과의 시간 차
{
    Super::Tick(DeltaTime);

    // 현재 액터의 Z 위치를 가져옴
    const double LocationZ = GetActorLocation().Z;

    // 만약 현재 위치가 원하는 위치보다 높다면
    if (LocationZ > DesiredZ) {
        // Z 방향으로 움직일 위치를 계산
        const FVector DeltaLocation = FVector(0.f, 0.f, DriftRate * DeltaTime);

        // 계산된 위치만큼 액터를 움직임
        AddActorWorldOffset(DeltaLocation);
    }
}

// 게임 시작이나 스폰될 때 호출되는 함수
void ASoul::BeginPlay()
{
    Super::BeginPlay();

    // 충돌 탐지를 위한 시작점과 끝점을 설정
    const FVector Start = GetActorLocation();
    const FVector End = Start - FVector(0.f, 0.f, 2000.f);

    // 객체 타입 설정
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

    // 무시할 액터 설정
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetOwner());

    // 충돌 결과를 저장할 변수
    FHitResult HitResult;

    // 라인 트레이스 실행
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

    // 원하는 Z 위치를 충돌 위치에 설정
    DesiredZ = HitResult.ImpactPoint.Z + 50.f;
}

// 오버랩 이벤트 발생 시 호출되는 함수
void ASoul::OnSphereOverlap(
    UPrimitiveComponent* OverlappedComponent,  // 오버랩된 컴포넌트
    AActor* OtherActor,  // 오버랩된 다른 액터
    UPrimitiveComponent* OtherComp,  // 오버랩된 다른 컴포넌트
    int32 OtherBodyIndex,  // 오버랩된 다른 바디 인덱스
    bool bFromSweep,  // 스윕으로부터의 오버랩 여부
    const FHitResult& SweepResult)  // 스윕 결과
{
    // 다른 액터를 픽업 인터페이스로 캐스팅
    IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

    // 만약 캐스팅이 성공했다면
    if (PickupInterface) {
        // 소울을 추가
        PickupInterface->AddSouls(this);

        // 픽업 효과와 사운드 생성
        SpawnPickupSystem();
        SpawnPickupSound();

        // 이 액터를 파괴
        Destroy();
    }
}

