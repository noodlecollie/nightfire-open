`convert.py` expects the output of running the Nightfire Workshop batch MDL conversion process to be placed within a `v14` folder next to the script. Within the `v14` folder, each model is expected to have its QC and SMD files in Half Life format, within a subdirectory named after the model.

The compiled models are put into a `v10` folder next to the script. Any errors are logged in `errors.log`, and log files for the relevant phases of the conversion can be found in `scratch/<model name>` with the extension `.log`.

`search-bsps.py` searches the entity lump of all BSPs in a directory. This was used to determine if any BSPs referenced a couple of specific MDL files (these files referenced textures that Nightfire did not appear to have).
