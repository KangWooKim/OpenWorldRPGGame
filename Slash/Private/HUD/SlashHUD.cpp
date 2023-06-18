// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"

// ASlashHUD�� BeginPlay �޼��� ����
void ASlashHUD::BeginPlay()
{
    // �θ� Ŭ������ BeginPlay ȣ��
    Super::BeginPlay();

    // ���� ����(World)�� �����ɴϴ�.
    UWorld* World = GetWorld();

    // ���谡 �����ϴ� ��쿡�� �Ʒ��� ������ �����մϴ�.
    if (World) {
        // ù ��° �÷��̾� ��Ʈ�ѷ��� �����ɴϴ�.
        APlayerController* Controller = World->GetFirstPlayerController();

        // ��Ʈ�ѷ��� �����ϸ�, SlashOverlayClass�� ���ǵǾ� �ִ� ��쿡�� �Ʒ��� ������ �����մϴ�.
        if (Controller && SlashOverlayClass) {
            // SlashOverlayClass���� SlashOverlay ������ �����մϴ�.
            SlashOverlay = CreateWidget<USlashOverlay>(Controller, SlashOverlayClass);
            // ������ SlashOverlay ������ ����Ʈ�� �߰��մϴ�.
            SlashOverlay->AddToViewport();
        }
    }
}

