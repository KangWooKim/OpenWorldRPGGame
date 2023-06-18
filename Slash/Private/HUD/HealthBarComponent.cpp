// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"

// UHealthBarComponent�� SetHealthPercent �޼��� ����
void UHealthBarComponent::SetHealthPercent(float Percent)
{
    // HealthBarWidget�� null�� ���, �ش� ��ü�� UHealthBar�� ĳ�����Ͽ� �����ɴϴ�.
    // Percent : ü�¹��� �ۼ�Ʈ(0.0 ~ 1.0)
    if (HealthBarWidget == nullptr) {
        HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
    }

    // HealthBarWidget�� ���� HealthBar�� �����Ѵٸ�,
    // HealthBar�� �ۼ�Ʈ�� �����մϴ�.
    if (HealthBarWidget && HealthBarWidget->HealthBar) {
        HealthBarWidget->HealthBar->SetPercent(Percent);
    }
}

