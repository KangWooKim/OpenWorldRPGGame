# Enemy.h

AEnemy::AEnemy(): 이는 생성자 함수로, 적 AI의 초기 설정을 수행합니다. 이 함수에서는 충돌 설정, 캐릭터 이동 방향 설정, 감지 컴포넌트의 초기 설정 등을 수행합니다.

AEnemy::Tick(float DeltaTime): 이 함수는 프레임마다 호출되며, 적 AI의 상태를 확인하고 적절한 행동을 취하도록 합니다.

AEnemy::TakeDamage(...): 이 함수는 적이 데미지를 받았을 때 호출되며, 적의 체력을 감소시키고 필요한 경우 행동 상태를 변경합니다.

AEnemy::Destroyed(): 이 함수는 적 AI 객체가 파괴되었을 때 호출되며, 적이 장착한 무기를 파괴하는 역할을 합니다.

AEnemy::GetHit_Implementation(...): 이 함수는 적이 공격을 받았을 때 호출되며, 공격 애니메이션을 중지하고 공격 타이머를 초기화합니다.

AEnemy::BeginPlay(): 이 함수는 적 AI 객체가 게임에 스폰되었을 때 한 번만 호출되며, 감지 컴포넌트를 초기화하고 적 AI를 설정합니다.

AEnemy::Die_Implementation(): 이 함수는 적 AI가 죽었을 때 호출되며, 적 AI의 상태를 "죽음"으로 설정하고 다양한 정리 작업을 수행합니다.

AEnemy::SpawnSoul(): 이 함수는 적 AI가 죽었을 때 영혼을 스폰하는 역할을 합니다.

AEnemy::Attack(): 이 함수는 적 AI가 공격을 시작할 때 호출됩니다.

AEnemy::CanAttack(): 이 함수는 적 AI가 공격할 수 있는 상태인지를 확인합니다.

AEnemy::AttackEnd(): 이 함수는 적 AI가 공격을 마친 후 호출되며, 적 AI의 상태를 변경합니다.

AEnemy::HandleDamage(float DamageAmount): 이 함수는 적 AI가 데미지를 받았을 때 호출되며, 체력바 위젯의 표시를 업데이트합니다.
