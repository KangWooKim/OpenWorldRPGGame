// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitinterface
{
	// ������ �ٵ�
	GENERATED_BODY()

public:
	// �� ������ �Ӽ��鿡 ���� �⺻�� ����
	ABreakableActor();

	// ��ݿ� ���� ó���� �ϴ� ���� �Լ�. FVector Ÿ���� ��� ������ AActor Ÿ���� ���͸� ���ڷ� �޽��ϴ�.
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	// ������ �����ϰų� ������ �� ȣ��Ǵ� ���� �Լ�
	virtual void BeginPlay() override;

	// GeometryCollection ������Ʈ. �������� �� �� ������, �������Ʈ���� �а� �� �� �ֽ��ϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UGeometryCollectionComponent* GeometryCollection;

	// Capsule ������Ʈ. �������� �� �� ������, �������Ʈ���� �а� �� �� �ֽ��ϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCapsuleComponent* Capsule;

public:
	// �� �����Ӹ��� ȣ��Ǵ� ���� �Լ�. ������ �ð��� DeltaTime�� ���ڷ� �޽��ϴ�.
	virtual void Tick(float DeltaTime) override;

private:
	// Breakable �Ӽ��� ���� ���� Ŭ���� �迭. �����Ϳ��� ��𼭳� ������ �� �ֽ��ϴ�.
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
		TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	// �� ���Ͱ� �μ������� ��Ÿ���� �Ҹ� ��. �ʱⰪ�� false�Դϴ�.
	bool bBroken = false;
};
