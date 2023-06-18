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

	// BeginPlay �޼ҵ� �������̵�. ���� �÷��̰� ���۵� �� ȣ��˴ϴ�.
	virtual void BeginPlay() override;

	
private:

	// SlashOverlayClass�� Slash ī�װ��� �����մϴ�. �ش� ������ �⺻ ������ ���� �����մϴ�.
	// TSubclassOf�� �̿��Ͽ� USlashOverlay�� ���� Ŭ������ �Ҵ��� �� �ֽ��ϴ�.
	UPROPERTY(EditDefaultsOnly, Category = Slash)
		TSubclassOf<USlashOverlay> SlashOverlayClass;

	// ������ ������ SlashOverlay ������ �����ϴ� �����Դϴ�.
	UPROPERTY()
		USlashOverlay* SlashOverlay;

	
public:

	// SlashOverlay�� ����(Getter) �޼ҵ��Դϴ�. �ش� �޼ҵ�� �ζ��� �޼ҵ�� ����Ǿ� �ֽ��ϴ�.
	// �� �޼ҵ带 ���� SlashOverlay ��� ������ ������ �� �ֽ��ϴ�.
	FORCEINLINE USlashOverlay* GetSlashOverlay() const { return SlashOverlay; }

	
};
