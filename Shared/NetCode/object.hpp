#pragma once

#include <Server/Components/Objects/objects.hpp>
#include <network.hpp>
#include <player.hpp>
#include <types.hpp>

namespace NetCode {
namespace RPC {
    struct SetPlayerObjectMaterial : NetworkPacketBase<84, NetworkPacketType::RPC> {
        int ObjectID;
        int MaterialID;
        ObjectMaterialData& MaterialData;

        SetPlayerObjectMaterial(ObjectMaterialData& materialData)
            : MaterialData(materialData)
        {
        }

        bool read(NetworkBitStream& bs)
        {
            return false;
        }

        void write(NetworkBitStream& bs) const
        {
            bs.writeUINT16(ObjectID);
            bs.writeUINT8(int(MaterialData.type));
            bs.writeUINT8(MaterialID);

            if (MaterialData.type == ObjectMaterialData::Type::Default) {
                bs.writeUINT16(MaterialData.model);
                bs.writeDynStr8(StringView(MaterialData.textOrTXD));
                bs.writeDynStr8(StringView(MaterialData.fontOrTexture));
                bs.writeUINT32(MaterialData.materialColour.ARGB());
            } else if (MaterialData.type == ObjectMaterialData::Type::Text) {
                bs.writeUINT8(MaterialData.materialSize);
                bs.writeDynStr8(StringView(MaterialData.fontOrTexture));
                bs.writeUINT8(MaterialData.fontSize);
                bs.writeUINT8(MaterialData.bold);
                bs.writeUINT32(MaterialData.fontColour.ARGB());
                bs.writeUINT32(MaterialData.backgroundColour.ARGB());
                bs.writeUINT8(MaterialData.alignment);
                bs.WriteCompressedStr(StringView(MaterialData.textOrTXD));
            }
        }
    };

    struct CreateObject : NetworkPacketBase<44, NetworkPacketType::RPC> {
        int ObjectID;
        int ModelID;
        Vector3 Position;
        Vector3 Rotation;
        float DrawDistance;
        bool CameraCollision;
        ObjectAttachmentData AttachmentData;
        StaticArray<ObjectMaterialData, MAX_OBJECT_MATERIAL_SLOTS> Materials;
        StaticBitset<MAX_OBJECT_MATERIAL_SLOTS>& MaterialsUsed;

        CreateObject(
            StaticArray<ObjectMaterialData, MAX_OBJECT_MATERIAL_SLOTS>& materials,
            StaticBitset<MAX_OBJECT_MATERIAL_SLOTS>& materialsUsed)
            : Materials(materials)
            , MaterialsUsed(materialsUsed)
        {
        }

        bool read(NetworkBitStream& bs)
        {
            return false;
        }

        void write(NetworkBitStream& bs) const
        {
            bs.writeUINT16(ObjectID);
            bs.writeINT32(ModelID);
            bs.writeVEC3(Position);
            bs.writeVEC3(Rotation);
            bs.writeFLOAT(DrawDistance);
            bs.writeUINT8(!CameraCollision);

            bs.writeUINT16(AttachmentData.type == ObjectAttachmentData::Type::Vehicle ? AttachmentData.ID : INVALID_VEHICLE_ID);
            bs.writeUINT16(AttachmentData.type == ObjectAttachmentData::Type::Object ? AttachmentData.ID : INVALID_OBJECT_ID);
            if (AttachmentData.type == ObjectAttachmentData::Type::Vehicle || AttachmentData.type == ObjectAttachmentData::Type::Object) {
                bs.writeVEC3(AttachmentData.offset);
                bs.writeVEC3(AttachmentData.rotation);
                bs.writeUINT8(AttachmentData.syncRotation);
            }

            bs.writeUINT8(MaterialsUsed.count());
            for (int i = 0; i < MaterialsUsed.size(); ++i) {
                if (MaterialsUsed.test(i)) {
                    const ObjectMaterialData& data = Materials[i];
                    bs.writeUINT8(int(data.type));
                    bs.writeUINT8(i);

                    if (data.type == ObjectMaterialData::Type::Default) {
                        bs.writeUINT16(data.model);
                        bs.writeDynStr8(StringView(data.textOrTXD));
                        bs.writeDynStr8(StringView(data.fontOrTexture));
                        bs.writeUINT32(data.materialColour.ARGB());
                    } else if (data.type == ObjectMaterialData::Type::Text) {
                        bs.writeUINT8(data.materialSize);
                        bs.writeDynStr8(StringView(data.fontOrTexture));
                        bs.writeUINT8(data.fontSize);
                        bs.writeUINT8(data.bold);
                        bs.writeUINT32(data.fontColour.ARGB());
                        bs.writeUINT32(data.backgroundColour.ARGB());
                        bs.writeUINT8(data.alignment);
                        bs.WriteCompressedStr(data.textOrTXD);
                    }
                }
            }
        }
    };

    struct DestroyObject : NetworkPacketBase<47, NetworkPacketType::RPC> {
        int ObjectID;

        bool read(NetworkBitStream& bs)
        {
            return false;
        }

        void write(NetworkBitStream& bs) const
        {
            bs.writeUINT16(ObjectID);
        }
    };

    struct MoveObject : NetworkPacketBase<99, NetworkPacketType::RPC> {
        int ObjectID;
        Vector3 CurrentPosition;
        ObjectMoveData MoveData;

        bool read(NetworkBitStream& bs)
        {
            return false;
        }

        void write(NetworkBitStream& bs) const
        {
            bs.writeUINT16(ObjectID);
            bs.writeVEC3(CurrentPosition);
            bs.writeVEC3(MoveData.targetPos);
            bs.writeFLOAT(MoveData.speed);
            bs.writeVEC3(MoveData.targetRot);
        }
    };

    struct StopObject : NetworkPacketBase<122, NetworkPacketType::RPC> {
        int ObjectID;

        bool read(NetworkBitStream& bs)
        {
            return false;
        }

        void write(NetworkBitStream& bs) const
        {
            bs.writeUINT16(ObjectID);
        }
    };

    struct SetObjectPosition : NetworkPacketBase<45, NetworkPacketType::RPC> {
        int ObjectID;
        Vector3 Position;

        bool read(NetworkBitStream& bs)
        {
            return false;
        }

        void write(NetworkBitStream& bs) const
        {
            bs.writeUINT16(ObjectID);
            bs.writeVEC3(Position);
        }
    };

    struct SetObjectRotation : NetworkPacketBase<46, NetworkPacketType::RPC> {
        int ObjectID;
        Vector3 Rotation;

        bool read(NetworkBitStream& bs)
        {
            return false;
        }

        void write(NetworkBitStream& bs) const
        {
            bs.writeUINT16(ObjectID);
            bs.writeVEC3(Rotation);
        }
    };

    struct AttachObjectToPlayer : NetworkPacketBase<75, NetworkPacketType::RPC> {
        int ObjectID;
        int PlayerID;
        Vector3 Offset;
        Vector3 Rotation;

        bool read(NetworkBitStream& bs)
        {
            return false;
        }

        void write(NetworkBitStream& bs) const
        {
            bs.writeUINT16(ObjectID);
            bs.writeUINT16(PlayerID);
            bs.writeVEC3(Offset);
            bs.writeVEC3(Rotation);
        }
    };

    struct SetPlayerAttachedObject : NetworkPacketBase<113, NetworkPacketType::RPC> {
        int PlayerID;
        int Index;
        bool Create;
        ObjectAttachmentSlotData AttachmentData;

        bool read(NetworkBitStream& bs)
        {
            return false;
        }

        void write(NetworkBitStream& bs) const
        {
            bs.writeUINT16(PlayerID);
            bs.writeUINT32(Index);
            bs.writeBIT(Create);
            if (Create) {
                bs.writeUINT32(AttachmentData.model);
                bs.writeUINT32(AttachmentData.bone);
                bs.writeVEC3(AttachmentData.offset);
                bs.writeVEC3(AttachmentData.rotation);
                bs.writeVEC3(AttachmentData.scale);
                bs.writeUINT32(AttachmentData.colour1.ARGB());
                bs.writeUINT32(AttachmentData.colour2.ARGB());
            }
        }
    };

    struct PlayerBeginObjectSelect : NetworkPacketBase<27, NetworkPacketType::RPC> {
        bool read(NetworkBitStream& bs)
        {
            return false;
        }

        void write(NetworkBitStream& bs) const
        {
        }
    };

    struct OnPlayerSelectObject : NetworkPacketBase<27, NetworkPacketType::RPC> {
        int SelectType;
        int ObjectID;
        int Model;
        Vector3 Position;

        bool read(NetworkBitStream& bs)
        {
            bs.readUINT32(SelectType);
            bs.readUINT16(ObjectID);
            bs.readUINT32(Model);
            return bs.readVEC3(Position);
        }

        void write(NetworkBitStream& bs) const
        {
        }
    };

    struct PlayerCancelObjectEdit : NetworkPacketBase<28, NetworkPacketType::RPC> {
        bool read(NetworkBitStream& bs)
        {
            return false;
        }

        void write(NetworkBitStream& bs) const
        {
        }
    };

    struct PlayerBeginObjectEdit : NetworkPacketBase<117, NetworkPacketType::RPC> {
        bool PlayerObject;
        int ObjectID;

        bool read(NetworkBitStream& bs)
        {
            return false;
        }

        void write(NetworkBitStream& bs) const
        {
            bs.writeBIT(PlayerObject);
            bs.writeUINT16(ObjectID);
        }
    };

    struct OnPlayerEditObject : NetworkPacketBase<117, NetworkPacketType::RPC> {
        bool PlayerObject;
        int ObjectID;
        int Response;
        Vector3 Offset;
        Vector3 Rotation;

        bool read(NetworkBitStream& bs)
        {
            bs.readBIT(PlayerObject);
            bs.readUINT16(ObjectID);
            bs.readUINT32(Response);
            bs.readVEC3(Offset);
            return bs.readVEC3(Rotation);
        }

        void write(NetworkBitStream& bs) const
        {
        }
    };

    struct PlayerBeginAttachedObjectEdit : NetworkPacketBase<116, NetworkPacketType::RPC> {
        unsigned Index;

        bool read(NetworkBitStream& bs)
        {
            return false;
        }

        void write(NetworkBitStream& bs) const
        {
            bs.writeUINT32(Index);
        }
    };

    struct OnPlayerEditAttachedObject : NetworkPacketBase<116, NetworkPacketType::RPC> {
        unsigned Response;
        unsigned Index;
        ObjectAttachmentSlotData AttachmentData;

        bool read(NetworkBitStream& bs)
        {
            bs.readUINT32(Response);
            bs.readUINT32(Index);
            bs.readUINT32(AttachmentData.model);
            bs.readUINT32(AttachmentData.bone);
            bs.readVEC3(AttachmentData.offset);
            bs.readVEC3(AttachmentData.rotation);
            bs.readVEC3(AttachmentData.scale);
            uint32_t argb;
            bs.readUINT32(argb);
            AttachmentData.colour1 = Colour::FromARGB(argb);
            bool res = bs.readUINT32(argb);
            AttachmentData.colour2 = Colour::FromARGB(argb);
            return res;
        }

        void write(NetworkBitStream& bs) const
        {
        }
    };
}
}
