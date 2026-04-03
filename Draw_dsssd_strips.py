import ROOT
import sys

FILE_NAME = "dsssd_calib_Str_r0944_r0945_r0946.root"
RUN_TAG   = "r0944_r0945_r0946"

# FILE_NAME = "dsssd_calib_Str_r1041_r1042_r1043.root"
# RUN_TAG   = "r1041_r1042_r1043"

root_file = ROOT.TFile.Open(FILE_NAME, "READ")
if not root_file or root_file.IsZombie():
    sys.exit(f"Cannot open {FILE_NAME}")

# collect available strip numbers by scanning canvas keys in the file
available_strips = sorted(set(
    int(file_key.GetName().split("_Str_")[-1].split(";")[0])
    for file_key in root_file.GetListOfKeys()
    if file_key.GetName().startswith(f"c_{RUN_TAG}_Str_")
))
print(f"Found {len(available_strips)} strips: {available_strips}")

current_index = [0]

canvas = ROOT.TCanvas("strip_browser", "DSSSD Strip Browser", 950, 700)
ROOT.gStyle.SetOptStat(0)

def draw_strip(strip_number):
    stored_canvas = root_file.Get(f"c_{RUN_TAG}_Str_{strip_number}")

    canvas.cd()
    canvas.Clear()

    if not stored_canvas:
        print(f"Strip {strip_number} not found in file")
        return

    stored_canvas.DrawClonePad()
    canvas.SetTitle(f"Strip {strip_number} - {RUN_TAG}")
    canvas.Modified()
    canvas.Update()

def go_to_previous():
    current_index[0] = max(0, current_index[0] - 1)
    draw_strip(available_strips[current_index[0]])

def go_to_next():
    current_index[0] = min(len(available_strips) - 1, current_index[0] + 1)
    draw_strip(available_strips[current_index[0]])

control_bar = ROOT.TControlBar("horizontal", "Navigation", 10, 10)
control_bar.AddButton("Prev", "TPython::Exec(\"go_to_previous()\")", "Go to previous strip")
control_bar.AddButton("Next", "TPython::Exec(\"go_to_next()\")",     "Go to next strip")
control_bar.Show()

draw_strip(available_strips[current_index[0]])
ROOT.gApplication.Run()