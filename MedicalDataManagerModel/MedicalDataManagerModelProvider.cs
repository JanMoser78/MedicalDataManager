using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using MedicalDataManagerModel.Annotations;

namespace MedicalDataManagerModel
{
    [Flags]
     enum ProtocolFlags
    {
        Urgent = 1,

    }

    public class MedicalDataManagerModelProvider
    {
        public void SaveAction(int actionid, string payload, DateTime timeStamp, string devUid)
        {
            int token = DecryptPayload(payload);
            using (var context = new MedicalDataManagerDataBaseContainer())
            {
                Action act = new Action();
                act.ActionTypeId = actionid;
                act.DeviceId = devUid;
                act.Payload = payload;
                act.TimeStamp = timeStamp;
                act.TokenId = token;
                context.Actions.Add(act);
                context.SaveChanges();
            }


        }

        public string ReturnAlarmdata(string payLoad)
        {
            int token = DecryptPayload(payLoad);
            using (var context = new MedicalDataManagerDataBaseContainer())
            {
                var personId = context.Tokens.Where(x => x.Id == token).FirstOrDefault();
                var pers = context.People.FirstOrDefault(x => x.Id == personId.Person_Id);
                return
                    string.Format("{0}, {1}, {2}, {3}, {4}", pers.Lastname, pers.Firstname, pers.PhoneNumber, pers.Email,
                        pers.Twitter);

            }
        }





        public string ReturnMedPlan(string DeviceID, int compartmentId)
        {
            return EncodePayLoad(DeviceID, compartmentId);
        }

        private string EncodePayLoad(string deviceId, int compartmentId)
        {
            using (var context = new MedicalDataManagerDataBaseContainer())
            {
                var medPlans = context.MedicationPlans.Where(x => x.DeviceId == deviceId && x.CompartmentNbr == compartmentId);
                var device = context.Devices.SingleOrDefault(x => x.Id == deviceId);
                if (device != null)
                {
                   
                   // List<string> allCompartmentDataOfDevice = new List<string>();
                    foreach (var medicationPlan in medPlans)
                    {
                        
                            short token = (short) medicationPlan.TokenId;
                            byte dosage = (byte) medicationPlan.DosageId;
                        long from = UnixTimestampFromDateTime(medicationPlan.ValidFrom);
                            long to = UnixTimestampFromDateTime(medicationPlan.Validto);
                            byte compartment = (byte) medicationPlan.CompartmentNbr;
                            byte critical = (byte) (medicationPlan.Critical ? 1 : 0);
                           
                            StringBuilder sb = new StringBuilder();
                            sb.Append(compartment.ToString("X2"));
                            sb.Append(token.ToString("X4"));
                            sb.Append(dosage.ToString("X2"));
                            sb.Append(from.ToString("X16"));
                            sb.Append(to.ToString("X16"));
                            sb.Append(critical.ToString("X2"));
                            return sb.ToString();
                        }
                        
                    
                   
                }
                
            }
            return  "00FFFF";
        }

        public static long UnixTimestampFromDateTime(DateTime date)
        {
            long unixTimestamp = date.Ticks - new DateTime(1970, 1, 1).Ticks;
            unixTimestamp /= TimeSpan.TicksPerSecond;
            return unixTimestamp*1000;
        }

        public int DecryptPayload(string payload)
        {
            int token = 0;
            string testString = "0104007b0003e8000000000000000000000000000000000000000000000000000000000000000000";
            char[] theChars = payload.ToCharArray();
            List<int> intVals = new List<int>();

            //ActionId
            char[] hexChar = new char[2];
            hexChar[0] = theChars[0];
            hexChar[1] = theChars[1];
            string val = new string(hexChar);
            intVals.Add((int)FromHex(val));
            
            //comId
           hexChar = new char[2];
            hexChar[0] = theChars[2];
            hexChar[1] = theChars[3];
             val = new string(hexChar);
            intVals.Add((int)FromHex(val));

            //token
            hexChar = new char[4];
            hexChar[0] = theChars[4];
            hexChar[1] = theChars[5];
            hexChar[2] = theChars[6];
            hexChar[3] = theChars[7];
            val = new string(hexChar);
            intVals.Add((int)FromHex(val));
            token = (int) FromHex(val);

            // timestamp
            hexChar = new char[16];
            hexChar[0] = theChars[16];
            hexChar[1] = theChars[17];
            hexChar[2] = theChars[18];
            hexChar[3] = theChars[19];
            hexChar[4] = theChars[20];
            hexChar[5] = theChars[21];
            hexChar[6] = theChars[22];
            hexChar[7] = theChars[23];
            hexChar[8] = theChars[24];
            hexChar[9] = theChars[25];
            hexChar[10] = theChars[26];
            hexChar[11] = theChars[27];
            hexChar[12] = theChars[28];
            hexChar[13] = theChars[29];
            hexChar[14] = theChars[30];
            hexChar[15] = theChars[31];

            val = new string(hexChar);
            long result = (FromHex(val));




            return token;


        }
        public static long FromHex(string value)
        {
            // strip the leading 0x
            if (value.StartsWith("0x", StringComparison.OrdinalIgnoreCase))
            {
                value = value.Substring(2);
            }
            return Int64.Parse(value, NumberStyles.HexNumber);
        }
        private byte[] StringToByteArray(string str)
        {
            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
            return enc.GetBytes(str);
        }

        private string ByteArrayToString(byte[] arr)
        {
            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
            return enc.GetString(arr);
        }
    }
}