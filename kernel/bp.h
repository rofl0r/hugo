/****************************************************************************/
/*                                                                          */
/*						Source file for handling Bp in core kernel                */
/*																									 */
/* For the user Bp, we just give the hand to the user                       */
/* For the GIVE_HAND_BP, we make it disappear and we give hand to user      */
/* For the RESTORE_BP, we set again the User BP placed old_user_bp in the   */
/* list then we make the current BP to disappear then set IP,...            */
/*                                                                          */
/* There are certainly a lot of improvement to do, If you got any idea :    */
/*           Zeograd@caramail.com                                           */
/*																									 */
/****************************************************************************/

case 0x0B:
case 0x1B:
case 0x2B:
case 0x3B:
case 0x4B:
case 0x5B:
case 0x6B:
case 0x7B:
case 0x8B:
case 0x9B:
case 0xAB:
case 0xBB:
case 0xCB:
case 0xDB:
	  // All user BreakPoints

	  M.PC.W--;
	  // Go to the place of the real instruction

#ifndef FINAL_RELEASE
	  if (M.PC.W!=Bp_list[I>>4].position)
		 fprintf(stderr,"there's a problem, the breakpoint hasn't been correctly hit\n");
     else
		 fprintf(stderr,"The breakpoint %d has been correctly hit\n",I>>4);

	  fprintf(stderr,"After Breakpoint, position is %X\n",M.PC.W);
#endif

	  disass_menu(M.PC.W);
	  // And call the disassembler

#ifndef FINAL_RELEASE

   fprintf(stderr,"After the disassembly function, the position is %X\n",M.PC.W);
#endif

     if ((Op6502(M.PC.W)&0x0B)==0x0B)
       {  // We only look here for Bp since PC or bp status can have changed

#ifndef FINAL_RELEASE

   fprintf(stderr,"run trick: a bp has been asked to be put at %X\n",M.PC.W);
#endif

	  _Wr6502(M.PC.W,Bp_list[Op6502(M.PC.W)>>4].original_op);
     // Replace the opcode in the rom

     Bp_list[Op6502(M.PC.W)>>4].flag=NOT_USED;
	  // Temporary, the breakpoint disappears
	  // to be replaced by the restore_bp

     Bp_pos_to_restore=M.PC.W;
     // We know we must restore a breakpoint at this point

     set_bp_following(M.PC.W,RESTORE_BP);
     // since we call this after disassembly call, we handle correctly
     // any changes in reg value e.g.

     }
	  break;

case 0xB+RESTORE_BP*0x10:

     M.PC.W--;
     // Go to the true IP

     // We must restore the Bp_to_restore Breakpoint

     _Wr6502(M.PC.W,Bp_list[I>>4].original_op);
     // Replace the opcode in the rom

     Bp_list[I>>4].flag=NOT_USED;
     // This BP is no more used

#ifndef FINAL_RELEASE
   fprintf(stderr,"We're restoring bp at %X\n",Bp_pos_to_restore);
#endif

     toggle_user_breakpoint(Bp_pos_to_restore);
     // We set another Bp at the location we just left

     break;

case 0xB+GIVE_HAND_BP*0x10:

   // We must make it disappear and call the disassembler

   M.PC.W--;
   // go to the good IP

   _Wr6502(M.PC.W,Bp_list[I>>4].original_op);
   // Replace the opcode in the rom

   Bp_list[I>>4].flag=NOT_USED;
   // This breakpoint is no more used

   disass_menu(M.PC.W);

   break;
