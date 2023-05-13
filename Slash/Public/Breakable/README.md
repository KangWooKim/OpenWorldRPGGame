# BreakableActor.h

ABreakableActor::ABreakableActor(): 이는 생성자 함수로, BreakableActor 객체가 생성될 때 호출됩니다. 생성자는 GeometryCollection 컴포넌트와 Capsule 컴포넌트를 생성하고 설정합니다. GeometryCollection은 루트 컴포넌트로 설정되며, 이 컴포넌트는 겹침 이벤트를 생성하도록 설정되고, 카메라와 폰에 대한 충돌 응답을 무시하도록 설정됩니다. Capsule 컴포넌트는 모든 채널에 대한 충돌 응답을 무시하도록 설정되며, 폰 채널에 대한 충돌 응답은 차단되도록 설정됩니다.

ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter): 이 함수는 BreakableActor가 공격받았을 때 호출됩니다. 만약 Actor가 이미 깨진 상태라면, 이 함수는 아무 일도 하지 않습니다. 그렇지 않으면, 이 함수는 Actor를 깨진 상태로 설정하고, 가능하면 세계에서 보물을 생성합니다. 보물의 클래스는 TreasureClasses 배열에서 무작위로 선택되며, 위치는 Actor의 위치에 조금 높은 곳으로 설정됩니다.

ABreakableActor::BeginPlay(): 이 함수는 게임이 시작하거나 Actor가 스폰될 때 호출됩니다. 현재 이 함수는 부모 클래스의 BeginPlay()를 호출하는 것 외에는 아무 일도 하지 않습니다.

ABreakableActor::Tick(float DeltaTime): 이 함수는 매 프레임마다 호출됩니다. 현재 이 함수는 부모 클래스의 Tick()을 호출하는 것 외에는 아무 일도 하지 않습니다.
