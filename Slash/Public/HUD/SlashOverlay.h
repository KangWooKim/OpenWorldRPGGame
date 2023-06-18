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
    // 체력 바의 퍼센트를 설정하는 메소드입니다.
    // @param Percent : 설정하려는 체력 퍼센트 값입니다.
    void SetHealthBarPercent(float Percent);

    // 스태미나 바의 퍼센트를 설정하는 메소드입니다.
    // @param Percent : 설정하려는 스태미나 퍼센트 값입니다.
    void SetStaminaBarPercent(float Percent);

    // 플레이어의 골드를 설정하는 메소드입니다.
    // @param Gold : 설정하려는 골드 값입니다.
    void SetGold(int32 Gold);

    // 플레이어의 소울을 설정하는 메소드입니다.
    // @param Souls : 설정하려는 소울 값입니다.
    void SetSouls(int32 Souls);

private:
    // 체력 바를 참조하는 포인터입니다. 이를 통해 체력 바 UI를 변경할 수 있습니다.
    UPROPERTY(meta = (BindWidget))
        class UProgressBar* HealthProgressBar;

    // 스태미나 바를 참조하는 포인터입니다. 이를 통해 스태미나 바 UI를 변경할 수 있습니다.
    UPROPERTY(meta = (BindWidget))
        class UProgressBar* StaminaProgressBar;

    // 골드를 표시하는 텍스트 블록을 참조하는 포인터입니다. 이를 통해 골드 UI를 변경할 수 있습니다.
    UPROPERTY(meta = (BindWidget))
        class UTextBlock* GoldText;

    // 소울을 표시하는 텍스트 블록을 참조하는 포인터입니다. 이를 통해 소울 UI를 변경할 수 있습니다.
    UPROPERTY(meta = (BindWidget))
        class UTextBlock* SoulsText;

	
};
