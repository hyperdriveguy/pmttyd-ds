"""Unskipped original demo presentation; isolated Azahar profile only.

Captures supplement the host's exact37-ID/guard assertions. Screenshots alone
are not automated assertions of dialogue, image fidelity, or original timing.
"""
import subprocess,time

def run(env,capture):
    def x(*args):return subprocess.check_output(['xdotool',*args],env=env,text=True,stderr=subprocess.DEVNULL).strip()
    x('windowfocus',x('search','--onlyvisible','--class','azahar').splitlines()[-1])
    started=time.monotonic()
    for at,name in [(18,'book-open'),(35,'first-page-t35'),(38,'first-page-t38'),(41,'first-page-t41'),(44,'first-page-t44'),(90,'last-storybook-page'),(110,'market-arrival'),(128,'peach-turn'),(160,'chest-lift'),(182,'walls-revealed'),(195,'chest-transfer'),(230,'final-title-guard')]:
        time.sleep(max(0,started+at-time.monotonic()));capture(name)
    def press(key):x('keydown',key);time.sleep(.10);x('keyup',key)
    press('b');time.sleep(1);capture('b-keeps-demo')
    press('a');time.sleep(16);capture('a-enters-title')
