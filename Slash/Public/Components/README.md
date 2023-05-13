# AttributeComponent.h

UAttributeComponent::UAttributeComponent(): 이는 생성자 함수로, AttributeComponent 객체가 생성될 때 호출됩니다. 현재로서는 PrimaryComponentTick의 bCanEverTick 프로퍼티를 false로 설정하는 것 외에는 아무런 작업도 수행하지 않습니다.

UAttributeComponent::BeginPlay(): 게임이 시작될 때 호출되는 함수입니다. 현재로서는 부모 클래스의 BeginPlay()를 호출하는 것 외에는 아무런 작업도 수행하지 않습니다.

UAttributeComponent::ReceiveDamage(float Damage): 이 함수는 캐릭터가 데미지를 받았을 때 호출됩니다. 전달된 데미지만큼 캐릭터의 Health를 감소시키며, Health가 0과 MaxHealth 사이의 값으로 유지되도록 합니다.

UAttributeComponent::UseStamina(float StaminaCost): 이 함수는 캐릭터가 스테미나를 사용할 때 호출됩니다. 전달된 스테미나 비용만큼 캐릭터의 Stamina를 감소시키며, Stamina가 0과 MaxStamina 사이의 값으로 유지되도록 합니다.

UAttributeComponent::GetHealthPercent(): 캐릭터의 현재 Health를 MaxHealth로 나눈 값을 반환하여 캐릭터의 Health 비율을 제공합니다.

UAttributeComponent::GetStaminaPercent(): 캐릭터의 현재 Stamina를 MaxStamina로 나눈 값을 반환하여 캐릭터의 Stamina 비율을 제공합니다.

UAttributeComponent::IsAlive(): 캐릭터의 생존 상태를 반환합니다. Health가 0보다 크면 true(살아있음)를 반환하고, 그렇지 않으면 false(죽음)를 반환합니다.

UAttributeComponent::AddSouls(int32 NumberOfSouls): 캐릭터의 현재 Souls에 전달된 수의 Souls를 추가합니다.

UAttributeComponent::AddGold(int AmountOfGold): 캐릭터의 현재 Gold에 전달된 수의 Gold를 추가합니다.

UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction): 매 프레임마다 호출되는 함수입니다. 현재로서는 부모 클래스의 TickComponent()를 호출하는 것 외에는 아무런 작업도 수행하지 않습니다.

UAttributeComponent::RegenStamina(float DeltaTime): 이 함수는 스테미나를 회복하는 데 사용됩니다. DeltaTime에 스테미나 재생률을 곱한 값을 현재 스테미나에 더하며, 이 결과 값이 0과 MaxStamina 사이의 값으로 유지되도록 합니다.
