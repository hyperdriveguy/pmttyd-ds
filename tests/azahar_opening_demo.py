"""Unskipped original demo presentation; isolated Azahar profile only.

Captures supplement the host's exact37-ID/guard assertions. Screenshots alone
are not automated assertions of dialogue, image fidelity, or original timing.
"""
import subprocess,time

def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    started=time.monotonic()
    for at,name in [(18,'book-open'),(84,'last-storybook-page'),(94,'market-arrival'),(110,'peach-turn'),(143,'chest-lift'),(160,'walls-revealed'),(175,'chest-transfer'),(195,'final-title-guard')]:
        time.sleep(max(0,started+at-time.monotonic()));capture(name)
    def press(key):x('keydown',key);time.sleep(.10);x('keyup',key)
    press('b');time.sleep(1);capture('b-keeps-demo')
    press('a');time.sleep(1);capture('a-enters-title')
