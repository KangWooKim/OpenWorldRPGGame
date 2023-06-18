// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"

// ASlashHUD의 BeginPlay 메서드 정의
void ASlashHUD::BeginPlay()
{
    // 부모 클래스의 BeginPlay 호출
    Super::BeginPlay();

    // 현재 세계(World)를 가져옵니다.
    UWorld* World = GetWorld();

    // 세계가 존재하는 경우에만 아래의 로직을 수행합니다.
    if (World) {
        // 첫 번째 플레이어 컨트롤러를 가져옵니다.
        APlayerController* Controller = World->GetFirstPlayerController();

        // 컨트롤러가 존재하며, SlashOverlayClass가 정의되어 있는 경우에만 아래의 로직을 수행합니다.
        if (Controller && SlashOverlayClass) {
            // SlashOverlayClass에서 SlashOverlay 위젯을 생성합니다.
            SlashOverlay = CreateWidget<USlashOverlay>(Controller, SlashOverlayClass);
            // 생성된 SlashOverlay 위젯을 뷰포트에 추가합니다.
            SlashOverlay->AddToViewport();
        }
    }
}

