// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API ATreasure : public AItem
{
	GENERATED_BODY()

protected:
	// 스피어 컴포넌트가 다른 컴포넌트와 겹쳤을 때 호출되는 함수
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	

private:
	// 플레이어에게 보상으로 지급할 Gold
	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold;
public:
	FORCEINLINE int32 GetGold() const { return Gold; }
};
