// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

// SetHealthBarPercent 메소드는 플레이어의 체력바의 퍼센트를 설정하는 함수입니다.
// 입력 값 Percent는 플레이어의 현재 체력 상태를 나타냅니다.
void USlashOverlay::SetHealthBarPercent(float Percent)
{
    // 체력바(HealthProgressBar)가 존재하면
    if (HealthProgressBar) {
        // 체력바의 퍼센트를 입력받은 Percent로 설정합니다.
        HealthProgressBar->SetPercent(Percent);
    }
}

// SetStaminaBarPercent 메소드는 플레이어의 스태미나바의 퍼센트를 설정하는 함수입니다.
// 입력 값 Percent는 플레이어의 현재 스태미나 상태를 나타냅니다.
void USlashOverlay::SetStaminaBarPercent(float Percent)
{
    // 스태미나바(StaminaProgressBar)가 존재하면
    if (StaminaProgressBar) {
        // 스태미나바의 퍼센트를 입력받은 Percent로 설정합니다.
        StaminaProgressBar->SetPercent(Percent);
    }
}

// SetGold 메소드는 플레이어의 현재 골드 수를 설정하는 함수입니다.
// 입력 값 Gold는 플레이어가 보유한 현재 골드 수를 나타냅니다.
void USlashOverlay::SetGold(int32 Gold)
{
    // 골드 표시 글자(GoldText)가 존재하면
    if (GoldText) {
        // 골드 수를 문자열로 변환합니다.
        const FString String = FString::Printf(TEXT("%d"), Gold);
        // 문자열을 FText 형태로 변환합니다.
        const FText Text = FText::FromString(String);
        // 골드 표시 글자를 위에서 생성한 텍스트로 설정합니다.
        GoldText->SetText(Text);
    }
}

// SetSouls 메소드는 플레이어의 현재 소울 수를 설정하는 함수입니다.
// 입력 값 Souls는 플레이어가 보유한 현재 소울 수를 나타냅니다.
void USlashOverlay::SetSouls(int32 Souls)
{
    // 소울 표시 글자(SoulsText)가 존재하면
    if (SoulsText) {
        // 소울 수를 문자열로 변환합니다.
        const FString String = FString::Printf(TEXT("%d"), Souls);
        // 문자열을 FText 형태로 변환합니다.
        const FText Text = FText::FromString(String);
        // 소울 표시 글자를 위에서 생성한 텍스트로 설정합니다.
        SoulsText->SetText(Text);
    }
}

