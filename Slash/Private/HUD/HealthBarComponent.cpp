// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"

// UHealthBarComponent의 SetHealthPercent 메서드 정의
void UHealthBarComponent::SetHealthPercent(float Percent)
{
    // HealthBarWidget이 null인 경우, 해당 객체를 UHealthBar로 캐스팅하여 가져옵니다.
    // Percent : 체력바의 퍼센트(0.0 ~ 1.0)
    if (HealthBarWidget == nullptr) {
        HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
    }

    // HealthBarWidget과 그의 HealthBar가 존재한다면,
    // HealthBar의 퍼센트를 설정합니다.
    if (HealthBarWidget && HealthBarWidget->HealthBar) {
        HealthBarWidget->HealthBar->SetPercent(Percent);
    }
}

