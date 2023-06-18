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
	// Percent로 HealthBar를 설정합니다.
	void SetHealthPercent(float Percent);

private:
	// 화면에 표시될 HealthBarWidget 입니다.
	UPROPERTY()
	class UHealthBar* HealthBarWidget;
};
