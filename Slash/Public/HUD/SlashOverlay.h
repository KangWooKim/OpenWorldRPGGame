// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlashOverlay.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API USlashOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
    // ü�� ���� �ۼ�Ʈ�� �����ϴ� �޼ҵ��Դϴ�.
    // @param Percent : �����Ϸ��� ü�� �ۼ�Ʈ ���Դϴ�.
    void SetHealthBarPercent(float Percent);

    // ���¹̳� ���� �ۼ�Ʈ�� �����ϴ� �޼ҵ��Դϴ�.
    // @param Percent : �����Ϸ��� ���¹̳� �ۼ�Ʈ ���Դϴ�.
    void SetStaminaBarPercent(float Percent);

    // �÷��̾��� ��带 �����ϴ� �޼ҵ��Դϴ�.
    // @param Gold : �����Ϸ��� ��� ���Դϴ�.
    void SetGold(int32 Gold);

    // �÷��̾��� �ҿ��� �����ϴ� �޼ҵ��Դϴ�.
    // @param Souls : �����Ϸ��� �ҿ� ���Դϴ�.
    void SetSouls(int32 Souls);

private:
    // ü�� �ٸ� �����ϴ� �������Դϴ�. �̸� ���� ü�� �� UI�� ������ �� �ֽ��ϴ�.
    UPROPERTY(meta = (BindWidget))
        class UProgressBar* HealthProgressBar;

    // ���¹̳� �ٸ� �����ϴ� �������Դϴ�. �̸� ���� ���¹̳� �� UI�� ������ �� �ֽ��ϴ�.
    UPROPERTY(meta = (BindWidget))
        class UProgressBar* StaminaProgressBar;

    // ��带 ǥ���ϴ� �ؽ�Ʈ ����� �����ϴ� �������Դϴ�. �̸� ���� ��� UI�� ������ �� �ֽ��ϴ�.
    UPROPERTY(meta = (BindWidget))
        class UTextBlock* GoldText;

    // �ҿ��� ǥ���ϴ� �ؽ�Ʈ ����� �����ϴ� �������Դϴ�. �̸� ���� �ҿ� UI�� ������ �� �ֽ��ϴ�.
    UPROPERTY(meta = (BindWidget))
        class UTextBlock* SoulsText;

	
};
