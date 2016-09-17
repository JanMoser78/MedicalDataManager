using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;

namespace MedicalDataManagerModel
{
    [Flags]
     enum ProtocolFlags
    {
        Urgent = 1,

    }
    public class MedicalDataManagerModelProvider
    {
        public void SaveAction(int actionid, string payload)
        {
            DecryptPayload(payload);

        }

        public string ReturnMedPlan(Guid DeviceID, int compartmentId)
        {
            return EncodePayLoad(Guid.Parse("83efe329-238b-4e9e-aa96-1f37e7c176ac"), 2);
        }

        private string EncodePayLoad(Guid deviceId, int compartmentId)
        {
            using (var context = new MedicalDataManagerDataBaseContainer())
            {
                var medPlans = context.MedicationPlans.Where(x => x.DeviceId == deviceId && x.CompartmentNbr == compartmentId);
                var device = context.Devices.SingleOrDefault(x => x.Id == deviceId);
                if (device != null)
                {
                    int nbrofCprtmnts = device.NbrOfCompartments;

                    int ctr = 0;
                   // List<string> allCompartmentDataOfDevice = new List<string>();
                    foreach (var medicationPlan in medPlans)
                    {
                        if (!(ctr < nbrofCprtmnts))
                        {
                            short token = (short) medicationPlan.TokenId;
                            byte dosage = (byte) medicationPlan.DosageId;
                            long from = medicationPlan.ValidFrom.Ticks;
                            long to = medicationPlan.Validto.Ticks;
                            byte compartment = (byte) medicationPlan.CompartmentNbr;
                            byte critical = (byte) (medicationPlan.Critical ? 1 : 0);
                            ctr++;
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
                
            }
            return  "00FFFF";
        }
        private Action DecryptPayload(string payload)
        {

            string testString = "04000000000000000000000000000000000000000000000000000000000000FF";
            char[] theChars = payload.ToCharArray();
            List<int> intVals = new List<int>();
            for (int i = 0; i < theChars.Length; i = i + 2)
            {
                char[] hexChar = new char[2];
                hexChar[0] = theChars[i];
                hexChar[1] = theChars[i + 1];
                string val = new string(hexChar);
                intVals.Add(FromHex(val));

            }
            return new Action();

        }
        public static int FromHex(string value)
        {
            // strip the leading 0x
            if (value.StartsWith("0x", StringComparison.OrdinalIgnoreCase))
            {
                value = value.Substring(2);
            }
            return Int32.Parse(value, NumberStyles.HexNumber);
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