// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

// SetHealthBarPercent �޼ҵ�� �÷��̾��� ü�¹��� �ۼ�Ʈ�� �����ϴ� �Լ��Դϴ�.
// �Է� �� Percent�� �÷��̾��� ���� ü�� ���¸� ��Ÿ���ϴ�.
void USlashOverlay::SetHealthBarPercent(float Percent)
{
    // ü�¹�(HealthProgressBar)�� �����ϸ�
    if (HealthProgressBar) {
        // ü�¹��� �ۼ�Ʈ�� �Է¹��� Percent�� �����մϴ�.
        HealthProgressBar->SetPercent(Percent);
    }
}

// SetStaminaBarPercent �޼ҵ�� �÷��̾��� ���¹̳����� �ۼ�Ʈ�� �����ϴ� �Լ��Դϴ�.
// �Է� �� Percent�� �÷��̾��� ���� ���¹̳� ���¸� ��Ÿ���ϴ�.
void USlashOverlay::SetStaminaBarPercent(float Percent)
{
    // ���¹̳���(StaminaProgressBar)�� �����ϸ�
    if (StaminaProgressBar) {
        // ���¹̳����� �ۼ�Ʈ�� �Է¹��� Percent�� �����մϴ�.
        StaminaProgressBar->SetPercent(Percent);
    }
}

// SetGold �޼ҵ�� �÷��̾��� ���� ��� ���� �����ϴ� �Լ��Դϴ�.
// �Է� �� Gold�� �÷��̾ ������ ���� ��� ���� ��Ÿ���ϴ�.
void USlashOverlay::SetGold(int32 Gold)
{
    // ��� ǥ�� ����(GoldText)�� �����ϸ�
    if (GoldText) {
        // ��� ���� ���ڿ��� ��ȯ�մϴ�.
        const FString String = FString::Printf(TEXT("%d"), Gold);
        // ���ڿ��� FText ���·� ��ȯ�մϴ�.
        const FText Text = FText::FromString(String);
        // ��� ǥ�� ���ڸ� ������ ������ �ؽ�Ʈ�� �����մϴ�.
        GoldText->SetText(Text);
    }
}

// SetSouls �޼ҵ�� �÷��̾��� ���� �ҿ� ���� �����ϴ� �Լ��Դϴ�.
// �Է� �� Souls�� �÷��̾ ������ ���� �ҿ� ���� ��Ÿ���ϴ�.
void USlashOverlay::SetSouls(int32 Souls)
{
    // �ҿ� ǥ�� ����(SoulsText)�� �����ϸ�
    if (SoulsText) {
        // �ҿ� ���� ���ڿ��� ��ȯ�մϴ�.
        const FString String = FString::Printf(TEXT("%d"), Souls);
        // ���ڿ��� FText ���·� ��ȯ�մϴ�.
        const FText Text = FText::FromString(String);
        // �ҿ� ǥ�� ���ڸ� ������ ������ �ؽ�Ʈ�� �����մϴ�.
        SoulsText->SetText(Text);
    }
}

