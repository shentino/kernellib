# include <kernel/kernel.h>
# include <kernel/user.h>

inherit LIB_WIZTOOL;


private object user;		/* associated user object */

/*
 * NAME:	create()
 * DESCRIPTION:	initialize object
 */
static create(int clone)
{
    if (clone) {
	::create(200);
	user = this_user();
    }
}

/*
 * NAME:	query_user()
 * DESCRIPTION:	return the current user
 */
static object query_user()
{
    return user;
}

/*
 * NAME:	input()
 * DESCRIPTION:	deal with input from user
 */
void input(string str)
{
    if (previous_object() == user) {
	call_limited("process", str);
    }
}

/*
 * NAME:	process()
 * DESCRIPTION:	process user input
 */
static void process(string str)
{
    string arg;

    if (query_editor(this_object())) {
	if (strlen(str) != 0 && str[0] == '!') {
	    str = str[1 ..];
	} else {
	    str = editor(str);
	    if (str) {
		user->message(str);
	    }
	    return;
	}
    }

    if (str == "") {
	return;
    }

    sscanf(str, "%s %s", str, arg);

    switch (str) {
    case "code":
    case "history":
    case "clear":
    case "compile":
    case "clone":
    case "destruct":

    case "cd":
    case "pwd":
    case "ls":
    case "cp":
    case "mv":
    case "rm":
    case "mkdir":
    case "rmdir":
    case "ed":

    case "access":
    case "grant":
    case "ungrant":
    case "quota":
    case "rsrc":

    case "people":
    case "status":
    case "swapout":
    case "statedump":
    case "shutdown":
    case "reboot":
	call_other(this_object(), "cmd_" + str, user, str, arg);
	break;

    default:
	user->message("No command: " + str + "\n");
	break;
    }
}
