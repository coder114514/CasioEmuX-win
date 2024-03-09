#include "StandbyControl.hpp"

#include "../Chipset/Chipset.hpp"
#include "../Chipset/MMU.hpp"
#include "../Emulator.hpp"
#include "../Logger.hpp"

namespace casioemu {
    // should be ignore read, but for the sake of debugging, this is easier to use.
    void StandbyControl::Initialise() {
        region_stpacp.Setup(
            0xF008, 1, "StandbyControl/STPACP", this, MMURegion::DefaultRead<uint8_t>, [](MMURegion *region, size_t, uint8_t data) {
                StandbyControl *self = (StandbyControl *)(region->userdata);
                if ((data & 0xF0) == 0xA0 && (self->stpacp_last & 0xF0) == 0x50) {
                    self->stop_acceptor_enabled = true;
                }
                self->stpacp_last = data;
            },
            emulator);

        region_sbycon.Setup(
            0xF009, 1, "StandbyControl/SBYCON", this, MMURegion::DefaultRead<uint8_t>, [](MMURegion *region, size_t, uint8_t data) {
                StandbyControl *self = (StandbyControl *)(region->userdata);

                if (data & 0x01) {
                    logger::Info("StandbyControl: Chipset halted!");
                    self->emulator.chipset.Halt();
                    return;
                }

                if (data & 0x02 && self->stop_acceptor_enabled) {
                    logger::Info("StandbyControl: Chipset stopped!");
                    self->stop_acceptor_enabled = false;
                    self->emulator.chipset.Stop();
                    return;
                }
            },
            emulator);
    }

    void StandbyControl::Reset() {
        stpacp_last = 0;
        stop_acceptor_enabled = false;
    }
}
