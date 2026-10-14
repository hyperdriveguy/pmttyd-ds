"""Unskipped original demo presentation; isolated Azahar profile only.

Captures supplement the host's exact37-ID/guard assertions. Screenshots alone
are not automated assertions of dialogue, image fidelity, or original timing.
"""
import subprocess,time

def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    started=time.monotonic()
    for at,name in [(18,'book-open'),(41,'first-page'),(90,'last-storybook-page'),(110,'market-arrival'),(128,'peach-turn'),(160,'chest-lift'),(182,'walls-revealed'),(195,'chest-transfer')]:
        time.sleep(max(0,started+at-time.monotonic()));capture(name)
    # No end-of-demo input: retain frequent captures across the automatic exit
    # and Title's idle replay window. Visual review locates the actual transition.
    for at in range(198,239,2):
        time.sleep(max(0,started+at-time.monotonic()));capture(f'auto-title-t{at}')
