# BaseCharacter.h


ABaseCharacter::ABaseCharacter(): 클래스의 생성자입니다. 여기서는 클래스의 초기 설정을 합니다. 각 속성을 초기화하고, 충돌 응답을 설정합니다.

ABaseCharacter::BeginPlay(): 게임이 시작될 때 호출되는 함수입니다.

ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter): 캐릭터가 공격을 받았을 때 호출되는 함수입니다. 공격받은 위치와 공격자를 파라미터로 받아, 해당 위치에서 피격 사운드와 파티클을 생성하고, 피격 애니메이션을 실행합니다.

ABaseCharacter::Attack(): 캐릭터가 공격하는 동작을 정의하는 함수입니다.

ABaseCharacter::Die_Implementation(): 캐릭터가 죽었을 때 호출되는 함수입니다. 죽음 애니메이션을 실행하고, 캐릭터에 "Dead" 태그를 추가합니다.

ABaseCharacter::PlayHitReactMontage(const FName& SectionName): 피격 반응 애니메이션을 재생하는 함수입니다. 애니메이션 섹션의 이름을 파라미터로 받습니다.

ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint): 공격받은 방향에 따라 피격 반응 애니메이션을 결정하고 재생하는 함수입니다.

ABaseCharacter::PlayHitSound(const FVector& ImpactPoint): 피격 사운드를 재생하는 함수입니다.

ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint): 피격 위치에 파티클을 생성하는 함수입니다.

ABaseCharacter::HandleDamage(float DamageAmount): 데미지를 처리하는 함수입니다.

ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName): 특정 애니메이션 몽타주의 특정 섹션을 재생하는 함수입니다.

ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames): 애니메이션 몽타주의 여러 섹션 중 하나를 무작위로 선택하여 재생하는 함수입니다.

ABaseCharacter::PlayAttackMontage(): 공격 애니메이션을 재생하는 함수입니다.

ABaseCharacter::PlayDeathMontage(): 죽음 애니메이션을 재생하는 함수입니다.

ABaseCharacter::PlayDodgeMontage(): 회피 애니메이션을 재생하는 함수입니다.
  
ABaseCharacter::StopAttackMontage(): 공격 애니메이션을 중지하는 함수입니다.

ABaseCharacter::GetTranslationWarpTarget(): 캐릭터가 이동하려는 위치를 계산하는 함수입니다. 이는 보통 전투 상황에서 적 캐릭터에게 접근하기 위해 사용됩니다.

ABaseCharacter::GetRotationWarpTarget(): 캐릭터가 회전하려는 목표 위치를 반환하는 함수입니다. 이는 보통 전투 상황에서 적 캐릭터를 바라보기 위해 사용됩니다.

ABaseCharacter::DisableCapsule(): 캐릭터의 캡슐 충돌체를 비활성화하는 함수입니다. 이는 주로 캐릭터가 죽었을 때 호출됩니다.

ABaseCharacter::CanAttack(): 캐릭터가 공격 가능한 상태인지를 판단하는 함수입니다.

ABaseCharacter::IsAlive(): 캐릭터가 살아있는지를 판단하는 함수입니다.

ABaseCharacter::DisableMeshCollision(): 캐릭터의 메시 충돌을 비활성화하는 함수입니다.

ABaseCharacter::AttackEnd(): 캐릭터의 공격이 끝났을 때 호출되는 함수입니다.

ABaseCharacter::DodgeEnd(): 캐릭터의 회피 동작이 끝났을 때 호출되는 함수입니다.

ABaseCharacter::Tick(float DeltaTime): 게임이 진행됨에 따라 매 프레임마다 호출되는 함수입니다.

ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled): 캐릭터의 무기 충돌을 설정하는 함수입니다. 이 함수는 무기가 다른 객체와 충돌할 수 있는지 여부를 설정합니다.
  
  ---
