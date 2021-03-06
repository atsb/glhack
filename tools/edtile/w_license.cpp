//------------------------------------------------------------------------
//  LICENSE Window
//------------------------------------------------------------------------
//
//  EdTile (C) 2001-2002 Andrew Apted
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//------------------------------------------------------------------------

using namespace std;

#include "z_config.h"

#include "w_license.h"
#include "w_base.h"


void LicenseWin::quit_but_callback(Fl_Widget *w, void *data)
{
  ASSERT(data);

  LicenseWin *licw = static_cast<LicenseWin *> (data);

  licw->SetQuit();
}


LicenseWin::LicenseWin() : WindowBase(0, 0, min_w, 390,
    PACKAGE_TITLE " License")
{
  size_range(min_w, min_h);

////  position(guix_prefs.manual_x, guix_prefs.manual_y);
 
  // allow manual closing of window
  callback(WindowBase::quit_callback);
  
  // create quit button in top row

  group = new Fl_Group(0, 0, w(), 34);
  group->end();
  group->resizable(0);
  add(group);
  
  quit = new Fl_Button(4, 4, 96, 26, "&Quit");
  quit->box(FL_THIN_UP_BOX);
  quit->callback((Fl_Callback *) quit_but_callback, this);
  group->add(quit);

  // create the browser

  int i;

  browser = new Fl_Browser(0, 34, w(), h() - 34);
 
  for (i=0; license_text[i]; i++)
    browser->add(license_text[i]);

  browser->position(0);

  add(browser);
  resizable(browser);

  // show the window
  set_modal();
  show();

  CaptureInitialPos();
}


//
// License Destructor
//
LicenseWin::~LicenseWin()
{
  // update preferences if user moved the window
#if 0
  if (x() != init_x || y() != init_y)
  {
    guix_prefs.manual_x = x();
    guix_prefs.manual_y = y();
  }

  if (w() != init_w || h() != init_h)
  {
    guix_prefs.manual_w = w();
    guix_prefs.manual_h = h();
  }
#endif
}


//------------------------------------------------------------------------

const char *LicenseWin::license_text[] =
{
"",
"@c@b@l GNU GENERAL PUBLIC LICENSE",
"",
"@c@b@m Version 2, June 1991",
"",
"@c Copyright (C) 1989, 1991 Free Software Foundation, Inc.",
"@c 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA",
"@c Everyone is permitted to copy and distribute verbatim copies",
"@c of this license document, but changing it is not allowed.",
"",
"",
"@-",
"",
"",
"@c@b Preamble",
"",
"@c   The licenses for most software are designed to take away your",
"@c freedom to share and change it.  By contrast, the GNU General Public",
"@c License is intended to guarantee your freedom to share and change free",
"@c software--to make sure the software is free for all its users.  This",
"@c General Public License applies to most of the Free Software",
"@c Foundation's software and to any other program whose authors commit to",
"@c using it.  (Some other Free Software Foundation software is covered by",
"@c the GNU Library General Public License instead.)  You can apply it to",
"@c your programs, too.",
"",
"@c   When we speak of free software, we are referring to freedom, not",
"@c price.  Our General Public Licenses are designed to make sure that you",
"@c have the freedom to distribute copies of free software (and charge for",
"@c this service if you wish), that you receive source code or can get it",
"@c if you want it, that you can change the software or use pieces of it",
"@c in new free programs; and that you know you can do these things.",
"",
"@c   To protect your rights, we need to make restrictions that forbid",
"@c anyone to deny you these rights or to ask you to surrender the rights.",
"@c These restrictions translate to certain responsibilities for you if you",
"@c distribute copies of the software, or if you modify it.",
"",
"@c   For example, if you distribute copies of such a program, whether",
"@c gratis or for a fee, you must give the recipients all the rights that",
"@c you have.  You must make sure that they, too, receive or can get the",
"@c source code.  And you must show them these terms so they know their",
"@c rights.",
"",
"@c   We protect your rights with two steps: (1) copyright the software, and",
"@c (2) offer you this license which gives you legal permission to copy,",
"@c distribute and/or modify the software.",
"",
"@c   Also, for each author's protection and ours, we want to make certain",
"@c that everyone understands that there is no warranty for this free",
"@c software.  If the software is modified by someone else and passed on, we",
"@c want its recipients to know that what they have is not the original, so",
"@c that any problems introduced by others will not reflect on the original",
"@c authors' reputations.",
"",
"@c   Finally, any free program is threatened constantly by software",
"@c patents.  We wish to avoid the danger that redistributors of a free",
"@c program will individually obtain patent licenses, in effect making the",
"@c program proprietary.  To prevent this, we have made it clear that any",
"@c patent must be licensed for everyone's free use or not licensed at all.",
"",
"@c   The precise terms and conditions for copying, distribution and",
"@c modification follow.",
"",
"",
"@-",
"",
"",
"@c@b@l GNU GENERAL PUBLIC LICENSE",
"",
"@c@b TERMS AND CONDITIONS FOR COPYING,",
"@c@b DISTRIBUTION AND MODIFICATION",
"",
"@c   0. This License applies to any program or other work which contains",
"@c a notice placed by the copyright holder saying it may be distributed",
"@c under the terms of this General Public License.  The \"Program\", below,",
"@c refers to any such program or work, and a \"work based on the Program\"",
"@c means either the Program or any derivative work under copyright law:",
"@c that is to say, a work containing the Program or a portion of it,",
"@c either verbatim or with modifications and/or translated into another",
"@c language.  (Hereinafter, translation is included without limitation in",
"@c the term \"modification\".)  Each licensee is addressed as \"you\".",
"",
"@c Activities other than copying, distribution and modification are not",
"@c covered by this License; they are outside its scope.  The act of",
"@c running the Program is not restricted, and the output from the Program",
"@c is covered only if its contents constitute a work based on the",
"@c Program (independent of having been made by running the Program).",
"@c Whether that is true depends on what the Program does.",
"",
"@c   1. You may copy and distribute verbatim copies of the Program's",
"@c source code as you receive it, in any medium, provided that you",
"@c conspicuously and appropriately publish on each copy an appropriate",
"@c copyright notice and disclaimer of warranty; keep intact all the",
"@c notices that refer to this License and to the absence of any warranty;",
"@c and give any other recipients of the Program a copy of this License",
"@c along with the Program.",
"",
"@c You may charge a fee for the physical act of transferring a copy, and",
"@c you may at your option offer warranty protection in exchange for a fee.",
"",
"@c   2. You may modify your copy or copies of the Program or any portion",
"@c of it, thus forming a work based on the Program, and copy and",
"@c distribute such modifications or work under the terms of Section 1",
"@c above, provided that you also meet all of these conditions:",
"",
"@c@s     a) You must cause the modified files to carry prominent notices",
"@c@s     stating that you changed the files and the date of any change.",
"",
"@c@s     b) You must cause any work that you distribute or publish, that in",
"@c@s     whole or in part contains or is derived from the Program or any",
"@c@s     part thereof, to be licensed as a whole at no charge to all third",
"@c@s     parties under the terms of this License.",
"",
"@c@s     c) If the modified program normally reads commands interactively",
"@c@s     when run, you must cause it, when started running for such",
"@c@s     interactive use in the most ordinary way, to print or display an",
"@c@s     announcement including an appropriate copyright notice and a",
"@c@s     notice that there is no warranty (or else, saying that you provide",
"@c@s     a warranty) and that users may redistribute the program under",
"@c@s     these conditions, and telling the user how to view a copy of this",
"@c@s     License.  (Exception: if the Program itself is interactive but",
"@c@s     does not normally print such an announcement, your work based on",
"@c@s     the Program is not required to print an announcement.)",
"",
"@c These requirements apply to the modified work as a whole.  If",
"@c identifiable sections of that work are not derived from the Program,",
"@c and can be reasonably considered independent and separate works in",
"@c themselves, then this License, and its terms, do not apply to those",
"@c sections when you distribute them as separate works.  But when you",
"@c distribute the same sections as part of a whole which is a work based",
"@c on the Program, the distribution of the whole must be on the terms of",
"@c this License, whose permissions for other licensees extend to the",
"@c entire whole, and thus to each and every part regardless of who wrote it.",
"",
"@c Thus, it is not the intent of this section to claim rights or contest",
"@c your rights to work written entirely by you; rather, the intent is to",
"@c exercise the right to control the distribution of derivative or",
"@c collective works based on the Program.",
"",
"@c In addition, mere aggregation of another work not based on the Program",
"@c with the Program (or with a work based on the Program) on a volume of",
"@c a storage or distribution medium does not bring the other work under",
"@c the scope of this License.",
"",
"@c   3. You may copy and distribute the Program (or a work based on it,",
"@c under Section 2) in object code or executable form under the terms of",
"@c Sections 1 and 2 above provided that you also do one of the following:",
"",
"@c@s     a) Accompany it with the complete corresponding machine-readable",
"@c@s     source code, which must be distributed under the terms of Sections",
"@c@s     1 and 2 above on a medium customarily used for software interchange; or,",
"",
"@c@s     b) Accompany it with a written offer, valid for at least three",
"@c@s     years, to give any third party, for a charge no more than your",
"@c@s     cost of physically performing source distribution, a complete",
"@c@s     machine-readable copy of the corresponding source code, to be",
"@c@s     distributed under the terms of Sections 1 and 2 above on a medium",
"@c@s     customarily used for software interchange; or,",
"",
"@c@s     c) Accompany it with the information you received as to the offer",
"@c@s     to distribute corresponding source code.  (This alternative is",
"@c@s     allowed only for noncommercial distribution and only if you",
"@c@s     received the program in object code or executable form with such",
"@c@s     an offer, in accord with Subsection b above.)",
"",
"@c The source code for a work means the preferred form of the work for",
"@c making modifications to it.  For an executable work, complete source",
"@c code means all the source code for all modules it contains, plus any",
"@c associated interface definition files, plus the scripts used to",
"@c control compilation and installation of the executable.  However, as a",
"@c special exception, the source code distributed need not include",
"@c anything that is normally distributed (in either source or binary",
"@c form) with the major components (compiler, kernel, and so on) of the",
"@c operating system on which the executable runs, unless that component",
"@c itself accompanies the executable.",
"",
"@c If distribution of executable or object code is made by offering",
"@c access to copy from a designated place, then offering equivalent",
"@c access to copy the source code from the same place counts as",
"@c distribution of the source code, even though third parties are not",
"@c compelled to copy the source along with the object code.",
"",
"@c   4. You may not copy, modify, sublicense, or distribute the Program",
"@c except as expressly provided under this License.  Any attempt",
"@c otherwise to copy, modify, sublicense or distribute the Program is",
"@c void, and will automatically terminate your rights under this License.",
"@c However, parties who have received copies, or rights, from you under",
"@c this License will not have their licenses terminated so long as such",
"@c parties remain in full compliance.",
"",
"@c   5. You are not required to accept this License, since you have not",
"@c signed it.  However, nothing else grants you permission to modify or",
"@c distribute the Program or its derivative works.  These actions are",
"@c prohibited by law if you do not accept this License.  Therefore, by",
"@c modifying or distributing the Program (or any work based on the",
"@c Program), you indicate your acceptance of this License to do so, and",
"@c all its terms and conditions for copying, distributing or modifying",
"@c the Program or works based on it.",
"",
"@c   6. Each time you redistribute the Program (or any work based on the",
"@c Program), the recipient automatically receives a license from the",
"@c original licensor to copy, distribute or modify the Program subject to",
"@c these terms and conditions.  You may not impose any further",
"@c restrictions on the recipients' exercise of the rights granted herein.",
"@c You are not responsible for enforcing compliance by third parties to",
"@c this License.",
"",
"@c   7. If, as a consequence of a court judgment or allegation of patent",
"@c infringement or for any other reason (not limited to patent issues),",
"@c conditions are imposed on you (whether by court order, agreement or",
"@c otherwise) that contradict the conditions of this License, they do not",
"@c excuse you from the conditions of this License.  If you cannot",
"@c distribute so as to satisfy simultaneously your obligations under this",
"@c License and any other pertinent obligations, then as a consequence you",
"@c may not distribute the Program at all.  For example, if a patent",
"@c license would not permit royalty-free redistribution of the Program by",
"@c all those who receive copies directly or indirectly through you, then",
"@c the only way you could satisfy both it and this License would be to",
"@c refrain entirely from distribution of the Program.",
"",
"@c If any portion of this section is held invalid or unenforceable under",
"@c any particular circumstance, the balance of the section is intended to",
"@c apply and the section as a whole is intended to apply in other",
"@c circumstances.",
"",
"@c It is not the purpose of this section to induce you to infringe any",
"@c patents or other property right claims or to contest validity of any",
"@c such claims; this section has the sole purpose of protecting the",
"@c integrity of the free software distribution system, which is",
"@c implemented by public license practices.  Many people have made",
"@c generous contributions to the wide range of software distributed",
"@c through that system in reliance on consistent application of that",
"@c system; it is up to the author/donor to decide if he or she is willing",
"@c to distribute software through any other system and a licensee cannot",
"@c impose that choice.",
"",
"@c This section is intended to make thoroughly clear what is believed to",
"@c be a consequence of the rest of this License.",
"",
"@c   8. If the distribution and/or use of the Program is restricted in",
"@c certain countries either by patents or by copyrighted interfaces, the",
"@c original copyright holder who places the Program under this License",
"@c may add an explicit geographical distribution limitation excluding",
"@c those countries, so that distribution is permitted only in or among",
"@c countries not thus excluded.  In such case, this License incorporates",
"@c the limitation as if written in the body of this License.",
"",
"@c   9. The Free Software Foundation may publish revised and/or new versions",
"@c of the General Public License from time to time.  Such new versions will",
"@c be similar in spirit to the present version, but may differ in detail to",
"@c address new problems or concerns.",
"",
"@c Each version is given a distinguishing version number.  If the Program",
"@c specifies a version number of this License which applies to it and \"any",
"@c later version\", you have the option of following the terms and conditions",
"@c either of that version or of any later version published by the Free",
"@c Software Foundation.  If the Program does not specify a version number of",
"@c this License, you may choose any version ever published by the Free",
"@c Software Foundation.",
"",
"@c   10. If you wish to incorporate parts of the Program into other free",
"@c programs whose distribution conditions are different, write to the author",
"@c to ask for permission.  For software which is copyrighted by the Free",
"@c Software Foundation, write to the Free Software Foundation; we sometimes",
"@c make exceptions for this.  Our decision will be guided by the two goals",
"@c of preserving the free status of all derivatives of our free software and",
"@c of promoting the sharing and reuse of software generally.",
"",
"@c@m NO WARRANTY",
"",
"@c@s   11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY",
"@c@s FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN",
"@c@s OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES",
"@c@s PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED",
"@c@s OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF",
"@c@s MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS",
"@c@s TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE",
"@c@s PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,",
"@c@s REPAIR OR CORRECTION.",
"",
"@c@s   12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING",
"@c@s WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR",
"@c@s REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,",
"@c@s INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING",
"@c@s OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED",
"@c@s TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY",
"@c@s YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER",
"@c@s PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE",
"@c@s POSSIBILITY OF SUCH DAMAGES.",
"",
"@c@b END OF TERMS AND CONDITIONS",
"",
NULL 
};

