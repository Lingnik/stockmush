//PLAYER:
//sk[ill]	(default)	:Displays skillsheet.
//	/show		:Same as (default).
//	/show <user>	:Show another user your skillsheet.
//	/showall	:See all skills against yours
//	/list		:See all skill categories.
//	/list <cat>	:See all skills and their descriptions in a category.
//ADMIN:
//sk[ill] <user>		:View another's skillsheet.
//	/view <user>	:Same as 'skill <user>'.
//	/showall <user>	:See all skills against user's.
//
//&CMD_TEST #266=$skill/test:@pe %#=[iter([sql(u(skill/sp_allusers),|,|)],%R[name(##)]:[iter([sql(u(sp_usertraits,##),|,^)],[@@(CODEGOESHERE)],|)],|)]


@dol #266 #933 #935=@wipe ##;&O ##=System object pointers.;&O`SQL ##=#933;&O`USR ##=#266;&O`ADM ##=#935;&O`FNC ##=#933


// ***###----------------###***
// ***### ADMIN COMMANDS ###***
// ***###----------------###***

// Command-set definitions:
// ASKILL	ADD`CATEGORY	ask[ill]/add cat <name>								Add a category.
// ASKILL	ADD`LEVEL		ask[ill]/add level <level>=<name>					Add a skill level.
// ASKILL	ADD`TRAIT		ask[ill]/add trait <category>/<name>				Add a trait to a category.
// ASKILL	ADD`USER		ask[ill]/add user <dbref>							Add a user to the skill system.
// ASKILL	DEL`CATEGORY	ask[ill]/del[ete] cat <name>						Delete a category.
// ASKILL	DEL`LEVEL		ask[ill]/del[ete] level <name>						Delete a skill level.
// ASKILL	DEL`TRAIT		ask[ill]/del[ete] trait <cat>/<name>				Delete a trait from a category.
// ASKILL	DEL`USER		ask[ill]/del[ete] user <dbref>						Delete a user from the skill system.
// ASKILL	RENAME`CATEGORY	ask[ill]/ren[ame] cat <old>=<new>					Rename a category.
// ASKILL	RENAME`LEVEL	ask[ill]/ren[ame] level <num>=<name>				Rename a skill level.
// ASKILL	RENAME`TRAIT	ask[ill]/ren[ame] trait <cat>/<old>=<cat>/<old>     Rename a trait. (Or move to another category.)
// ASKILL	DESC`CATEGORY	ask[ill]/desc[ribe] cat <name>=<description>		Describe a category.
// ASKILL	DESC`TRAIT		ask[ill]/desc[ribe] trait <cat>/<name>=<desc		Describe a trait in a category.
// ASKILL	SET				ask[ill]/set <user>/<infofield>=<value>				Set an informational field on a user.
// ASKILL	CLEAR			ask[ill]/clear <user>/<infofield>					Clear an informational field on a user.
// ASKILL	GIVE			ask[ill]/give <user>=<level>=<cat>/<trait>			Assign a trait to a user at a skill level.
// ASKILL	TAKE			ask[ill]/take <user>=<cat>/<trait>					Take a trait away from a user.
// ASKILL	LIST			ask[ill]/raw <table:cat|level|trait|user>			View the raw contents of a table.
// ASKILL	SHOWALL			ask[ill]/showall <user>								View a user's long sheet.
// ASKILL	VIEW			ask[ill]/show <user>								View a user's abbreviated sheet.

// Regexp definitions:
// ask[ill]/		$^(as|ask|aski|askil|askill)/
// del[ete]			del(e|et|ete)?
// <level>			(\d+)
// <{char}>			(.+)
// <{dbref}>		#(\d+)
// <table:...>		(cat(e|eg|ego|egor|egory|egori|egorie|egories)?|level(s)?|trait(s)?|user(s)?)


&CMD #935=User-defined commands.
&CMD`ASKILL_ADD #935=Commands to add entries to cf_ sql tables.
&CMD`ASKILL_ADD`CATEGORY #935=$^(as|ask|aski|askil|askill)/add category (.+)=(.+)$:
&CMD`ASKILL_ADD`LEVEL #935=$^(as|ask|aski|askil|askill)/add level (\d+)=(.+)$:
&CMD`ASKILL_ADD`TRAIT #935=$^(as|ask|aski|askil|askill)/add trait (.+)=(.+)$:
&CMD`ASKILL_ADD`USER #935=$^(as|ask|aski|askil|askill)/add user (.+)$:
&CMD`ASKILL_GIVE #935=$^(as|ask|aski|askil|askill)/give (.+)=(.+):(.+)$:
&CMD`ASKILL_LIST #935=$^(as|ask|aski|askil|askill)/list( (.+))?$:
&CMD`ASKILL_SHOWALL #935=$^(as|ask|aski|askil|askill)/showall (.+)$:@nspemit %#=[switch([u(F`CHECK_CAN_VIEW_OTHERS,%#)],1,[switch([pmatch(%2)],#-1,SKILL: Player not found.,#-2,SKILL: Which player do you mean?,[u(F`ANSITEXT_SHOWALL_LIST,[pmatch(%2)])])],0,SKILL: You do not have permission to do that.)]
&CMD`ASKILL_VIEW #935=$^(as|ask|aski|askil|askill)(/view)? (.+)$:@nspemit %#=[switch([u(F`CHECK_CAN_VIEW_OTHERS,%#)],1,[switch([pmatch(%3)],#-1,SKILL: Player not found.,#-2,SKILL: Which player do you mean?,[u(F`ANSITEXT_SHOW_LIST,[pmatch(%3)])])],0,SKILL: You do not have permission to do that.)]
@dol [lattr(#935/CMD`ASKILL_**)]=@set #266/##=regexp


// skill[/show]             View your abbreviated sheet.
// skill[/show] <user>      Show another person your abbreviated sheet.
// skill/showall            View your full sheet.
// skill/list [<category>]  List categories. [Or list traits in a category.]
// ***### USER COMMANDS ###***
&CMD #266=User-defined commands.
&CMD`SKILL_SHOW #266=View the sheet of a character.
&CMD`SKILL_SHOW`MINE #266=$^(sk|ski|skil|skill)(/show)?$:@nspemit %#=[u(F`ANSITEXT_SHOW_LIST,%#)]
&CMD`SKILL_SHOW`TO_USER #266=$^(sk|ski|skil|skill)(/show) (.+)$:@nspemit %#=[switch([pmatch(%3)],#-1,SKILL: Player not found.,#-2,SKILL: Which player do you mean?,SKILL: Showing [name(#$)] your sheet.[nspemit(#$,SKILL: [ansi(gh,[name(%#)] is showing you their sheet!:%R[u(F`ANSITEXT_SHOW_LIST,%#)])])]
&CMD`SKILL_SHOW`MINE_ALL #266=$^(sk|ski|skil|skill)/showall$:@nspemit %#=[u(F`ANSITEXT_SHOWALL_LIST,%#)]
&CMD`SKILL_LIST #266=View the list of categories and traits in those categories.
&CMD`SKILL_LIST`CATEGORIES #266=$^(sk|ski|skil|skill)/list$:@nspemit %#=[u(F`ANSITEXT_LIST_CAT)]
&CMD`SKILL_LIST`TRAITS #266=$^(sk|ski|skil|skill)/list (\w+)$:@nspemit %#=[switch(u(F`CHECK_CATEGORY_EXISTS,%0),1,u(F`ANSITEXT_LIST_SKL,%2),0,SKILL: Category does not exist.)]
@dol [lattr(#266/CMD`SKILL_**)]=@set #266/##=regexp


// ***### SHARED FUNCTIONS ###***
&F #933=Shared function()s.
&F`CHECK #933=Boolean checks for various things.
&F`CHECK`CATEGORY_EXISTS #933=1[nspemit(#23,[ansi(rh,TODO:)] check_category_exists for skills)]
&F`CHECK`CAN_VIEW_OTHERS #933=[ISTHEME(%0)]
&F`CHECK`CAN_EDIT_OTHERS #933=[ISTHEME(%0)]
&F`CHECK`THEME #933=[ISTHEME(%0)]
&F`TEXT #933=Formatted output.
&F`TEXT`SHOW_LIST #933=<ansitext_show_list:%0>
&F`TEXT`SHOWALL_LIST #933=<ansitext_showall_list:%0>
&F`TEXT`LIST_CAT #933=<ansitext_list_cat>
&F`TEXT`LIST_SKL #933=<ansitext_list_skl:%0>


// ***### SHARED DATA ACCESSORS ###***
&SP #933=Stored SQL procedures. (Sort of.)
&SP`allusertraits #933=SELECT cf_user.dbref, cf_trait.category, cf_trait.name, cf_usertrait.level FROM cf_user, cf_usertrait, cf_trait WHERE cf_user.dbref = cf_usertrait.dbref AND cf_usertrait.traitid = cf_trait.traitid ORDER BY cf_user.dbref, cf_trait.category, cf_trait.name
&SP`alltraits #933=SELECT cf_trait.category, cf_trait.name, cf_trait.description from cf_trait
&SP`allusers #933=SELECT cf_user.dbref FROM cf_user ORDER BY cf_user.dbref
&SP`usertraits #933=SELECT cf_usertrait.dbref, cf_trait.category, cf_trait.name, cf_usertrait.level FROM cf_usertrait, cf_trait WHERE (cf_usertrait.traitid = cf_trait.traitid AND cf_usertrait.dbref = [v(0)]) ORDER BY cf_trait.category, cf_trait.name
&SP`insert_category #933=INSERT INTO cf_category (categoryid, name, description) VALUES (0, %0, %1);
&SP`insert_level #933=INSERT INTO cf_levels (levelid, name) VALUES (0, %0);
&SP`insert_trait_raw #933=INSERT INTO cf_trait (traitid, categoryid, name) VALUES (0, %0, %1);
&SP`insert_user #933=INSERT INTO cf_user (dbref) VALUES (%0);
&SP`insert_usertrait_raw #933=INSERT INTO cf_usertrait (dbref, traitid, levelid) VALUES (%0, %1, %2);
&SP`insert_trait #933=[u(SP`insert_trait_raw,[sql(select traitid from cf_trait where cf_trait.name = '%0';)],%1)]
&SP`insert_usertrait #933=[u(SP`insert_usertrait_raw,%0, [sql(select traitid from cf_trait where cf_trait.name = '%1';)], [sql(select levelid from cf_levels where cf_levels.name = '%2';)])]


// ***### CODE DOCUMENTATION ###***
&DOC #933=System documentation.
&DOC`F`CHECK_CATEGORY_EXISTS #933=in string; out 1|0
&DOC`F`CHECK_CAN_VIEW_OTHERS #933=in dbref; out 1|0
&DOC`F`CHECK_CAN_EDIT_OTHERS #933=in dbref; out 1|0
&DOC`F`ANSITEXT_SHOW_LIST #933=in dbref - user to retrieve; out ansitext - formatted list
&DOC`F`ANSITEXT_SHOWALL_LIST #933=in dbref - user to retrieve; out ansitext - formatted list
&DOC`F`ANSITEXT_LIST_CAT #933=out ansitext - formatted list
&DOC`F`ANSITEXT_LIST_SKL #933=in string - category to display; out ansitext - formatted list
&DOC`SP`usertraits #933=in dbref; out listtraits
&DOC`SP`insert_category #933=in name(20), in description
&DOC`SP`insert_level #933=in name(20)
&DOC`SP`insert_trait_raw #933=in categoryid, in name(45)
&DOC`SP`insert_user #933=in dbref
&DOC`SP`insert_usertrait_raw #933=in dbref, in traitid, in levelid
&DOC`SP`insert_trait #933=in categoryname, in name(45)
&DOC`SP`insert_usertrait #933=in dbref, in traitname, in levelname

// ***### EXTERNAL CODE ###***
&FUNC_ISTHEME #741=[@@(in dbref; out 1|0)][or(orlflags(%0,WIZARD ROYALTY THEME))]


//&FUNC_ISWIZ #741=[@@(in dbref; out 1|0)][hasflag(%0,WIZARD)]
//&FUNC_ISADMIN #741=[@@(in dbref; out 1|0)][or(orlflags(%0,WIZARD ROYALTY JUDGE),haspower(%0,chat_privs))]
//&FUNC_ISJUDGE #741=[@@(in dbref; out 1|0)][orflags(%0,JUDGE)]
//&FUNC_ISJUROR #741=[@@(in dbref; out 1|0)][orlflags(%0,JURY_OK)]
//&FUNC_ISCODER #741=[@@(in dbref; out 1|0)][haspower(%0,functions)]
//&FUNC_ISBUILDER #741=[@@(in dbref; out 1|0)][haspower(%0,builder)]

// Powers are as follows:
//  builder: Designates a 'builder.'
//  functions: Designates a 'coder.'
//  chat_privs: Designates a non-bitted 'admin.'
// Bits are as follows:
//  WIZARD: A wizard. SysAdmin, Warden, Overseer. Possibly- Coder.
//  ROYALTY: Possibly- Coder, Architect.
//  JUDGE: Player chosen to arbitrate OOC issues.
//  JURY_OK: Active player randomly selected for jury duty.
//  THEME: Designates a 'theme' admin.


			// Ply   Adm  Title       Notes
			// 81    -8		GameMaster	Admin, private.
			// 81    +8		Architect	Admin, public.
			// 9=	 		Guide		Helper, public.
			// j	 		Juror		Juror, secret, temporary.
			// 93	 J9		Judge		Judge, private.
			// 91    r9		Overseer	Royalty, public.
			// 72*   		Builder		Builder, unk, temp.
			// 72* 	 		Coder		Admin, unk, temp.
			// 70    W		SysAdmin	Wizard, public.
			// ?0	 W?		Warden		Wizard, public.
			// 
			// Flags: GAMEMASTER(-), ARCHITECT(_), GUIDE(=)
			// *: Powers




// Power structure of the staff of the MUSH:

//    Operations Warden-----Theme Warden-----Support Warden
//           |                    |             |       |
//        Overseers           Architects        |       |
//   .-------+-------.            |             |       |
// Judges  Jurors  Guides    Game Masters    Coders  Builders
// 
// (SysAdmin is a Wizard-bitted character but does not have a ranking in this
//  diagram unless they hold another position.)


//Staff Guidelines and Descriptions:
// GameMaster- Pool of shared accounts; same password. GMs may use the Theme channel from their character account, and the Staff channel from their GM account.
// Architect- Character account is publicised as being Architect, and no additional account is made. Architects may use the Admin, Staff, and Theme channels.
// Guide- Character account is publicised as being Guide, and no additional account is made. Guides may use the Help channel. Certain +join/+return commands and others are at their disposal for duties.
// Juror- As needed, pool of potential Jurors is selected, and available Jurors are chosen. Jurors are joined to Jury channel until duties are complete.
// Judge- Pool of shared accounts; same password. Judges may use the Admin and Judges channels from their character account, and the Staff channel from their Judge account.
// Overseer- Royalty account is made for person. They may use the Staff and Jury channel from their roy account, and the Admin, Staff, and Judge channels from either account.
// Builder- Character account is endowed with builder powers and functions for duties. May use the Staff channel.
// Coder- Character account is endowed with programming powers and functions for duties. May use the Staff channel.
// SysAdmin- Wizard account is made for person. They may use the Wizard, Admin, or Staff channels from either their Character Account or Wizard Account.
// Warden- Wizard account is made for person. They may use the Wizard channel from their Character or Wizard Accounts, and the Admin and Staff channels through their Wizard Account.
// Ops Warden- Can access Jury, Judges channels via Wizard Account.
// Theme Warden- Can access Theme channel via Wizard Account.


// Flags to set on game staff:
// POSITION    PLAYER CHARACTER FLAGS         ADMIN CHARACTER FLAGS
// Warden:     <WTHM, WOPS, or WSUP>, PWIZ    <WTHM, WOPS, or WSUP>, WIZARD
// SysAdmin:   WSUP, PWIZ                     WIZARD
// Coder:      WSUP, PSTF, [Powers]           (Not Applicable. Same Character.)
// Builder:    WSUP, PSTF, [Powers]           (Not Applicable. Same Character.)
// Overseer:   WOPS, PADM                     WOPS, WIZARD
// Judge:      WOPS, PADM, PJDG               WOPS, JUDGE
// Juror:      JUROR                          (Not Applicable. Same Character.)
// Guide:      WOPS, GUIDE                    (Not Applicable. Same Character.)
// Architect:  WTHM, PADM                     WTHM, ARCHITECT
// GameMaster: WTHM, PADM                     WTHM, GAME_MASTER
//
// All of these must be set by a wizard (warden) except:
//  PWIZ and WIZARD can only be set by GOD.
//  GUIDE may be set by royalty.

// Flag creation commands:

@dol JURY_OK JUROR THEME_ADMIN GAME_MASTER ARCHITECT GUIDE PWIZ PADM PSTF PJDG PTHM PROY WSUP WTHM WOPS=@flag/delete ##

@flag/add GUIDE==,player,royalty,royalty
@flag/add JUROR=j,player,wizard mdark log,wizard
@flag/restrict JUDGE=wizard mdark log,wizard

@flag/add GAME_MASTER=-,player,wizard odark log,wizard
@flag/add ARCHITECT=+,player,wizard odark log,wizard

@flag/add PWIZ=0,player,god mdark log,god
@flag/add PADM=1,player,wizard mdark log,wizard
@flag/add PSTF=2,player,wizard mdark log,wizard
@flag/add PJDG=3,player,wizard mdark log,wizard

@flag/add WSUP=7,player,wizard mdark log,wizard
@flag/add WTHM=8,player,wizard mdark log,wizard
@flag/add WOPS=9,player,wizard mdark log,wizard

// Channel Permissions and checks:
// <Wizard>: {W0}		Wardens(Wizards), SysAdmin
&FUN_ISCHANWIZARD #1027=orlflags(%0,WIZARD PWIZ)
// <Admin>:  {W0r1J3+}	<Wizard>, Overseers, Judges(Admin|PC), Architects
&FUN_ISCHANADMIN #1027=orlflags(%0,WIZARD PWIZ ROYALTY PADM JUDGE PJDG ARCHITECT)
// <Staff>:  {W0r1J+-2}	<Admin>, GMs(Admin Only), Judges(Admin Only), Builders, Coders
&FUN_ISCHANSTAFF #1027=orlflags(%0,WIZARD PWIZ ROYALTY PADM JUDGE ARCHITECT GAME_MASTER PSTF)
// <Help>:   {*}		Guides, [Everyone]
&FUN_ISHELP #1027=hasflag(%0,GUIDE)
// <Jury>:   <9{W0r1j}>	Operations Warden, Overseers, Jurors
&FUN_ISCHANJUROR #1027=or(and(hasflag(%0,WOPS),orlflags(%0,WIZARD PWIZ ROYALTY PADM)),hasflag(%0,JUROR))
// <Judges>: <9{W0r1J}>	Operations Warden, Overseers, Judges
&FUN_ISCHANJUDGE #1027=and(hasflag(%0,WOPS),orlflags(%0,WIZARD PWIZ ROYALTY PADM JUDGE PJDG))
// <Theme>:  {8}		Theme Warden, Architects, GMs
&FUN_ISCHANTHEME #1027=hasflag(%0,WTHM)

