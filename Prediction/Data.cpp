
#include "Data.h"

namespace IPrediction
{
    Data* Data::m_instance = nullptr;

    Data::Data()
    {
        this->api = API::Get();
        this->InitDashSpells();
        this->InitExclusionBuffs();
        this->InitImmunityBuffs();
        this->InitImmobilityBuffs();
        this->InitImmobilityTypes();
    }

    void Data::Destroy()
    {
        delete m_instance;
        m_instance = nullptr;
    }

    Data* Data::Get()
    {
        if (m_instance == nullptr)
        {
            m_instance = new Data();
        }
        return m_instance;
    }

    // Global data functions

    const DashTable& Data::GetDashSpells()
    {
        return this->dashes;
    }

    const BuffTable& Data::GetExclusionBuffs()
    {
        return this->exclusions;
    }

    const BuffTable& Data::GetImmunityBuffs()
    {
        return this->immunities;
    }

    const BuffTable& Data::GetImmobilityBuffs()
    {
        return this->immobile_buffs;
    }

    const BuffTypes& Data::GetImmobilityTypes()
    {
        return this->immobile_types;
    }

    // Data initialization

    void Data::InitDashSpells()
    {
        this->dashes = DashTable();

        auto add_to_table = [&](DashData& data)
        {
            this->dashes[data.SpellName] = data;
        };

        // Akali E

        DashData data;
        data.SpellName = "AkaliE";
        data.Delay = 0.15f;
        data.Range = 0.0f;
        data.Offset = -400.0f;
        data.Speed = 1500.0f;
        data.FixedRange = true;
        add_to_table(data);

        // Aurora E

        data = DashData();
        data.SpellName = "AuroraE";
        data.Delay = 0.35f;
        data.Range = 0.0f;
        data.Offset = -250.0f;
        data.Speed = 820.0f;
        data.FixedRange = true;
        add_to_table(data);

        // Caitlyn E

        data = DashData();
        data.SpellName = "CaitlynE";
        data.Delay = 0.15f;
        data.Range = 0.0f;
        data.Offset = -390.0f;
        data.Speed = 600.0f;
        data.FixedRange = true;
        add_to_table(data);

        // Ezreal E

        data = DashData();
        data.SpellName = "EzrealE";
        data.Delay = 0.25f;
        data.Range = 475.0f;
        data.IsBlink = true;
        add_to_table(data);

        // FiddleSticks R

        data = DashData();
        data.SpellName = "FiddleSticksR";
        data.Delay = 1.5f;
        data.Range = 800.0f;
        data.IsBlink = true;
        add_to_table(data);

        // Graves R

        data = DashData();
        data.SpellName = "GravesChargeShot";
        data.Delay = 0.25f;
        data.Range = 0.0f;
        data.Offset = -400.0f;
        data.Speed = 1150.0f;
        data.FixedRange = true;
        add_to_table(data);

        // Kassadin R

        data = DashData();
        data.SpellName = "Riftwalk";
        data.Delay = 0.25f;
        data.Range = 500.0f;
        data.IsBlink = true;
        add_to_table(data);

        // Ornn E

        data = DashData();
        data.SpellName = "OrnnE";
        data.Delay = 0.35f;
        data.Range = 650.0f;
        data.Speed = 1600.0f;
        data.FixedRange = true;
        add_to_table(data);

        // Pyke R

        data = DashData();
        data.SpellName = "PykeR";
        data.Delay = 0.5f;
        data.Range = 750.0f;
        data.IsBlink = true;
        add_to_table(data);

        // Shaco Q

        data = DashData();
        data.SpellName = "Deceive";
        data.Delay = 0.125f;
        data.Range = 400.0f;
        data.IsBlink = true;
        add_to_table(data);

        // Shyvana R

        data = DashData();
        data.SpellName = "ShyvanaTransformCast";
        data.Delay = 0.25f;
        data.Range = 850.0f;
        data.Speed = 1100.0f;
        add_to_table(data);

        // TahmKench W

        data = DashData();
        data.SpellName = "TahmKenchW";
        data.Delay = 0.75f;
        data.Range = 1200.0f;
        data.IsBlink = true;
        add_to_table(data);

        // Tristana W

        data = DashData();
        data.SpellName = "TristanaW";
        data.Delay = 0.25f;
        data.Range = 900.0f;
        data.Speed = 1100.0f;
        add_to_table(data);

        // TwistedFate R

        data = DashData();
        data.SpellName = "Gate";
        data.Delay = 1.5f;
        data.Range = 5500.0f;
        data.IsBlink = true;
        add_to_table(data);

        // Urgot E

        data = DashData();
        data.SpellName = "UrgotE";
        data.Delay = 0.45f;
        data.Range = 450.0f;
        data.Speed = 1500.0f;
        data.FixedRange = true;
        add_to_table(data);

        // Viego R

        data = DashData();
        data.SpellName = "ViegoR";
        data.Delay = 0.5f;
        data.Range = 500.0f;
        data.IsBlink = true;
        add_to_table(data);
    }

    void Data::InitExclusionBuffs()
    {
        this->exclusions =
        {
            this->api->FNV1A32("EliseSpiderE"),
            this->api->FNV1A32("fizzeicon"),
            this->api->FNV1A32("galioemove"),
            this->api->FNV1A32("nocturneparanoiadash"),
            this->api->FNV1A32("YoneR"),
            this->api->FNV1A32("ZedR2")
        };
    }

    void Data::InitImmunityBuffs()
    {
        this->immunities =
        {
            this->api->FNV1A32("bardrstasis"),
            this->api->FNV1A32("ekkorinvuln"),
            this->api->FNV1A32("GwenW"),
            this->api->FNV1A32("LissandraRSelf"),
            this->api->FNV1A32("MelWReflect"),
            this->api->FNV1A32("PantheonE"),
            this->api->FNV1A32("VladimirSanguinePool"),
            this->api->FNV1A32("ChronoRevive"),
            this->api->FNV1A32("ZhonyasRingShield")
        };
    }

    void Data::InitImmobilityBuffs()
    {
        this->immobile_buffs =
        {
            this->api->FNV1A32("bardrstasis"),
            this->api->FNV1A32("CaitlynR"),
            this->api->FNV1A32("ekkorinvuln"),
            this->api->FNV1A32("KarthusFallenOne"),
            this->api->FNV1A32("LissandraRSelf"),
            this->api->FNV1A32("MalzaharR"),
            this->api->FNV1A32("PantheonR"),
            this->api->FNV1A32("ShenR"),
            this->api->FNV1A32("VelkozR"),
            this->api->FNV1A32("ChronoRevive"),
            this->api->FNV1A32("ZhonyasRingShield")
        };
    }

    void Data::InitImmobilityTypes()
    {
        this->immobile_types =
        {
            BuffType::Asleep,
            BuffType::Charm,
            BuffType::Fear,
            BuffType::Flee,
            BuffType::Knockup,
            BuffType::Snare,
            BuffType::Stun,
            BuffType::Suppression,
            BuffType::Taunt
        };
    };
}
