# HealthBarComponent.h

UHealthBarComponent::SetHealthPercent(float Percent) : HealthBar의 양을 float Percent로 설정합니다.

---

# SlashOverlay.h

USlashOverlay::SetHealthBarPercent(float Percent): 이 함수는 플레이어의 체력 바의 퍼센트를 설정하는 역할을 합니다. 인자로 전달된 퍼센트 값을 체력 바 위젯에 반영합니다.

USlashOverlay::SetStaminaBarPercent(float Percent): 이 함수는 플레이어의 스태미나 바의 퍼센트를 설정하는 역할을 합니다. 인자로 전달된 퍼센트 값을 스태미나 바 위젯에 반영합니다.

USlashOverlay::SetGold(int32 Gold): 이 함수는 플레이어의 골드 수량을 설정하는 역할을 합니다. 인자로 전달된 골드 수량을 골드 텍스트 위젯에 반영합니다.

USlashOverlay::SetSouls(int32 Souls): 이 함수는 플레이어의 영혼 수량을 설정하는 역할을 합니다. 인자로 전달된 영혼 수량을 영혼 텍스트 위젯에 반영합니다.

이 클래스는 게임 HUD (Head-Up Display)의 일부인 SlashOverlay에 대한 인터페이스를 제공합니다. 이는 플레이어의 현재 체력, 스태미나, 골드, 영혼 수량 등을 표시하고 업데이트하는데 사용됩니다.
