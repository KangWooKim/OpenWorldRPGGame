// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	// Percent�� HealthBar�� �����մϴ�.
	void SetHealthPercent(float Percent);

private:
	// ȭ�鿡 ǥ�õ� HealthBarWidget �Դϴ�.
	UPROPERTY()
	class UHealthBar* HealthBarWidget;
};
