#!/usr/bin/env python3
"""Check read-only US ground homing identity and selected follower branches."""
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_field_lecture_callbacks import verify

if __name__ == '__main__':
    result = verify(DEFAULT_ISO, Path(__file__).with_name('field_partner_homing_evidence.json'))
    print(f"Verified {result['body_checks']} homing body, {result['instruction_checks']} selected instructions; static evidence only")
