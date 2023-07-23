#!/usr/bin/env bash

set -eo pipefail

if [ -z "$SERENITY_SOURCE_DIR" ]
then
    echo "SERENITY_SOURCE_DIR is not set. Exiting."
    exit 1
fi

update_expectations_metadata=false
extra_parameters=""

while test $# -gt 0
do
    case "$1" in
        --update-expectations-metadata) update_expectations_metadata=true
            ;;
        --setup)
            # Clone patched web-platform-tests repository
            git clone --depth 10000 https://github.com/web-platform-tests/wpt.git

            # Switch to the commit that was used to generate tests expectations. Requires periodic updates.
            (cd wpt; git checkout 4434e91bd0801dfefff044b5b9a9744e30d255d3)

            # Apply WPT patch with Ladybird runner
            (cd wpt; git apply ../ladybird_runner.patch)

            # Update hosts file
            ./wpt/wpt make-hosts-file | sudo tee -a /etc/hosts

            # Extract metadata.txt into directory with expectation files expected by WPT runner
            ./concat-extract-metadata.py --extract metadata.txt metadata
            ;;
        *)
            extra_parameters="${extra_parameters} $1"
            ;;
    esac
    shift
done

# NOTE: WPT runner assumes Ladybird, WebContent and WebDriver are available in $PATH.
export PATH="${SERENITY_SOURCE_DIR}/Build/lagom/bin:${SERENITY_SOURCE_DIR}/Meta/Lagom/Build/bin:${PATH}"

# Generate name for file with wpt run log
wpt_run_log_filename="$(mktemp).txt"

# Run tests.
./wpt/wpt run ladybird $extra_parameters --no-fail-on-unexpected --no-fail-on-unexpected-pass --skip-timeout --include-manifest include.ini --metadata ./metadata --manifest ./MANIFEST.json --log-raw "${wpt_run_log_filename}"

# Update expectations metadata files if requested
if [[ $update_expectations_metadata == true ]]; then
    ./wpt/wpt update-expectations --product ladybird --metadata ./metadata --manifest ./MANIFEST.json "${wpt_run_log_filename}"
    ./concat-extract-metadata.py --concat ./metadata > metadata.txt
fi
