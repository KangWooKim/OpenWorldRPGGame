# Weapon.h

AWeapon::AWeapon(): 이 생성자 함수는 AWeapon 오브젝트가 생성될 때 호출됩니다. 여기서 무기의 충돌체를 설정하고, 충돌 응답을 정의합니다.

AWeapon::BeginPlay(): 이 함수는 게임 플레이가 시작될 때 호출됩니다. 여기서 무기의 Box 컴포넌트가 다른 오브젝트와 겹쳤을 때 호출되는 함수를 설정합니다.

AWeapon::Equip(...): 이 함수는 무기를 장착할 때 호출됩니다. 이는 장착 상태를 설정하고, 오브젝트의 소유자를 설정하며, 메시를 소켓에 부착하는 등의 작업을 수행합니다.

AWeapon::DeactivateEmbers(): 이 함수는 아이템 효과를 비활성화하는 역할을 합니다.

AWeapon::DisableSphereCollision(): 이 함수는 Sphere 컴포넌트의 충돌을 비활성화하는 역할을 합니다.

AWeapon::PlayEquipSound(): 이 함수는 무기를 장착할 때 소리를 재생하는 역할을 합니다.

AWeapon::AttachMeshToSocket(...): 이 함수는 무기의 메시를 특정 소켓에 부착하는 역할을 합니다.

AWeapon::OnBoxOverlap(...): 이 함수는 무기의 Box 컴포넌트가 다른 오브젝트와 겹쳤을 때 호출됩니다. 이 함수는 피해를 적용하고, 피해를 입은 오브젝트에 대한 추가적인 작업을 수행합니다.

AWeapon::ActorIsSameType(AActor* OtherActor): 이 함수는 전달된 오브젝트가 무기의 소유자와 동일한 타입인지 확인하는 역할을 합니다.

AWeapon::ExecuteGetHit(FHitResult& BoxHit): 이 함수는 피해를 입은 오브젝트가 'IHitinterface'를 구현하고 있는 경우, 해당 오브젝트에 피해를 입히는 역할을 합니다.

AWeapon::BoxTrace(FHitResult& BoxHit): 이 함수는 무기에서 Box Trace를 실행하는 역할을 합니다. 이는 특정 지점에서 다른 지점까지의 경로에서 오브젝트를 찾는 데 사용됩니다.
