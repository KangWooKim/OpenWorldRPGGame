// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlashHUD.generated.h"

/**
 * 
 */

class USlashOverlay;
UCLASS()



class SLASH_API ASlashHUD : public AHUD
{
	GENERATED_BODY()

		
protected:

	// BeginPlay 메소드 오버라이드. 게임 플레이가 시작될 때 호출됩니다.
	virtual void BeginPlay() override;

	
private:

	// SlashOverlayClass를 Slash 카테고리로 정의합니다. 해당 변수는 기본 설정만 편집 가능합니다.
	// TSubclassOf를 이용하여 USlashOverlay의 서브 클래스만 할당할 수 있습니다.
	UPROPERTY(EditDefaultsOnly, Category = Slash)
		TSubclassOf<USlashOverlay> SlashOverlayClass;

	// 실제로 생성된 SlashOverlay 위젯을 저장하는 변수입니다.
	UPROPERTY()
		USlashOverlay* SlashOverlay;

	
public:

	// SlashOverlay의 게터(Getter) 메소드입니다. 해당 메소드는 인라인 메소드로 선언되어 있습니다.
	// 이 메소드를 통해 SlashOverlay 멤버 변수에 접근할 수 있습니다.
	FORCEINLINE USlashOverlay* GetSlashOverlay() const { return SlashOverlay; }

	
};
